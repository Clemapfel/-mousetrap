//
// Created by clem on 3/31/23.
//

#include <mousetrap.hpp>
#include "julia_interface.cpp"
#include <jlcxx/jlcxx.hpp>

using namespace mousetrap;

#define add_method(Type, id) method(#id, &Type::id)
#define declare_is_subtype_of(Super, Sub) template<> struct jlcxx::SuperType<Super> { typedef Sub type; };
#define make_not_mirrored(Name) template<> struct jlcxx::IsMirroredType<Name> : std::false_type {};

// SignalEmitter
make_not_mirrored(AbstractSignalEmitter)

template<typename T, typename Arg_t>
void make_signal_emitter(Arg_t& type, jlcxx::Module& module)
{
    type
        .add_method(T, set_signal_blocked)
        .add_method(T, get_all_signal_names)
        //.add_method(T, connect_signal)
        .add_method(T, disconnect_signal)
        .add_method(T, new_signal)
    ;
}

// Widget
make_not_mirrored(AbstractWidget)

template<typename T, typename Arg_t>
void make_widget(Arg_t& type, jlcxx::Module& module)
{
    type
        .add_method(T, set_margin_top)
        .add_method(T, get_margin_top)
        .add_method(T, set_margin_bottom)
        .add_method(T, set_margin_start)
        .add_method(T, get_margin_start)
        .add_method(T, set_margin_end)
        .add_method(T, get_margin_end)
        .add_method(T, set_margin_horizontal)
        .add_method(T, set_margin_vertical)
        .add_method(T, set_margin)
    ;

    module.method("set_size_request", [](T& in, float width, float height) -> void {
       in.Widget::set_size_request({width, height});
    });

    module.method("get_size_request", [](T& in) -> jl_value_t* {
        auto size = in.Widget::get_size_request();
        auto* new_vector2f = jl_get_function((jl_module_t*) jl_get_global(jl_main_module, jl_symbol("mousetrap")), "Vector2f");

        if (new_vector2f == nullptr)
            log::fatal("In Widget::get_size_request: Unable to retrieve Julia function pointer to Vector2f(x, y)");

        auto* out = jl_call2(new_vector2f, jl_box_float64(size.x), jl_box_float64(size.y));
        return out == nullptr ? jl_nothing : out;
    });
}

// Application
void add_application(jlcxx::Module& module)
{
    auto application = module.add_type<Application>("Application", jlcxx::julia_base_type<SignalEmitter>())
        .constructor<const std::string&>()
        .add_method(Application, run)
        .add_method(Application, quit)
        .add_method(Application, hold)
        .add_method(Application, release)
        .add_method(Application, mark_as_busy)
        .add_method(Application, unmark_as_busy)
        //.add_method(Application, add_action)
        //.add_method(Application, remove_action)
        //.add_method(Application, get_action)
        //.add_method(Application, has_action)
        //.add_method(Application, set_menubar)
        .add_method(Application, get_id)
    ;
    make_signal_emitter<Application>(application, module);

    module.method("connect_signal_activate", [](Application& instance, jl_function_t* julia_function){

        if (not jl_isa(julia_function, (jl_value_t*) jl_function_type))
            log::critical("In Application::connect_signal_activate: Argument #1 is not a function", MOUSETRAP_DOMAIN);

        instance.connect_signal_activate([f = julia_function](Application* instance) {

            auto* app = jlcxx::box<Application&>(*instance).value;
            std::cout << jl_to_string(app) << std::endl;
            auto* out = jl_call1(f, app);

            /*
            if (out == nullptr)
            {
                log_critical("In Application::emit_signal_activate: Unable to invoke Julia object `", jl_to_string(f), "` as function with signature `(Application) -> Cvoid`");
                return false;
            }
             */
        });
    });
}

declare_is_subtype_of(Application, AbstractSignalEmitter)

// Window
void add_window(jlcxx::Module& module)
{
    auto window = module.add_type<Window>("Window", jlcxx::julia_base_type<AbstractWidget>())
        .constructor<>()
        .constructor<Application&>()
        .add_method(Window, set_application)
        .add_method(Window, set_maximized)
        .add_method(Window, set_fullscreen)
        .add_method(Window, present)
        .add_method(Window, set_show_menubar)
        .add_method(Window, set_hide_on_close)
        .add_method(Window, close)
        .add_method(Window, set_child)
        .add_method(Window, get_child)
        .add_method(Window, set_destroy_with_parent)
        .add_method(Window, get_destroy_with_parent)
        .add_method(Window, set_title)
        .add_method(Window, get_title)
        .add_method(Window, set_titlebar_layout)
        .add_method(Window, set_titlebar_widget)
        .add_method(Window, get_titlebar_widget)
        .add_method(Window, set_is_modal)
        .add_method(Window, get_is_modal)
        .add_method(Window, set_transient_for)
        .add_method(Window, set_is_decorated)
        .add_method(Window, get_is_decorated)
        .add_method(Window, set_has_close_button)
        .add_method(Window, get_has_close_button)
        .add_method(Window, set_startup_notification_identifier)
        .add_method(Window, set_focus_visible)
        .add_method(Window, get_focus_visible)
    ;
    make_widget<Window>(window, module);
    make_signal_emitter<Window>(window, module);
}

declare_is_subtype_of(Window, AbstractWidget)

// Action
void add_action(jlcxx::Module& module)
{
    auto action = module.add_type<Action>("Action")
        .constructor<std::string>(true)
        .add_method(Action, get_id)
        .add_method(Action, set_state)
        .add_method(Action, get_state)
        .add_method(Action, activate)
        .add_method(Action, set_enabled)
        .add_method(Action, get_enabled)
        .add_method(Action, get_is_stateful);

    module.method("set_function", [](Action& action, jl_function_t* julia_function){

        if (not jl_isa(julia_function, (jl_value_t*) jl_function_type))
            log::critical("In Action::set_function: Argument #2 is not a function", MOUSETRAP_DOMAIN);

        action.set_function([f = julia_function, id = action.get_id()](){
            auto* out = jl_call0(f);
            if (out == nullptr)
                log_critical("In Action::activate: Action with id `", id, "`: Unable to invoke Julia object `", jl_to_string(f), "` as function with signature `() -> Cvoid`");
        });
    });

    module.method("set_stateful_function", [](Action& action, jl_function_t* julia_function, bool initial_state)
    {
        if (not jl_isa(julia_function, (jl_value_t*) jl_function_type))
            log::critical("In Action::set_stateful_function: Argument #2 is not a function", MOUSETRAP_DOMAIN);

        action.set_stateful_function([f = julia_function, id = action.get_id()](bool in) -> bool {

            auto* out = jl_call1(f, jl_box_bool(in));

            if (out == nullptr or not jl_isa(out, (jl_value_t*) jl_bool_type))
            {
                log_critical("In Action::activate: Action with id `", id, "`: Unable to invoke Julia object `", jl_to_string(f), "` as function with signature `(Bool) -> Bool`");
                return false;
            }

            return jl_unbox_bool(out);
        });
    });
}

JLCXX_MODULE define_julia_module(jlcxx::Module& module)
{


    module.add_type<AbstractWidget>("AbstractWidget");
    module.add_type<Widget>("Widget");

    module.add_type<AbstractSignalEmitter>("AbstractSignalEmitter");
    module.add_type<SignalEmitter>("SignalEmitter");

    add_application(module);
    add_window(module);
    add_action(module);
}
