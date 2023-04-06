//
// Created by clem on 3/31/23.
//

#include <mousetrap.hpp>
#include <jlcxx/jlcxx.hpp>

using namespace mousetrap;

#define add_method(Type, id) method(#id, &Type::id)
#define declare_is_supertype(Super, Sub) template<> struct jlcxx::SuperType<Sub> { typedef Super type; };

// SignalEmitter
template<typename T, typename Arg_t>
void make_signal_emitter(Arg_t& type)
{
    type
        .add_method(T, set_signal_blocked)
        .add_method(T, get_all_signal_names)
        //.add_method(T, connect_signal)
        .add_method(T, disconnect_signal)
        .add_method(T, new_signal)
    ;
}

declare_is_supertype(SignalEmitter, Widget)

// Widget
template<typename T, typename Arg_t>
void make_widget(Arg_t& type)
{
    type
        //.add_method(T, get_size_request)
        //.add_method(T, set_size_request)
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
}

JLCXX_MODULE define_julia_module(jlcxx::Module& module)
{
    module.add_type<Widget>("Widget");
    module.add_type<SignalEmitter>("SignalEmitter");
    module.add_type<ApplicationID>("ApplicationID").constructor<std::string>();

    // Application
    auto application = module.add_type<Application>("Application")
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
    make_signal_emitter<Application>(application);

    // Window
    auto window = module.add_type<Window>("Window")
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
    make_widget<Window>(window);
    make_signal_emitter<Window>(window);
}
