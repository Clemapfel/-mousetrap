//
// Created by clem on 3/31/23.
//

#include <mousetrap.hpp>
#include "~julia_interface.cpp"
#include <jlcxx/jlcxx.hpp>

using namespace mousetrap;

#define add_type_method(Type, id) method(#id, &Type::id)
#define add_global_method(name) method(#name, mousetrap::name)
#define declare_is_subtype_of(A, B) template<> struct jlcxx::SuperType<A> { typedef B type; };
#define make_not_mirrored(Name) template<> struct jlcxx::IsMirroredType<Name> : std::false_type {};
#define add_enum(Name) add_bits<Name>(#Name, jlcxx::julia_type("CppEnum"))
#define add_enum_value(EnumName, PREFIX, VALUE_NAME) set_const(std::string(#PREFIX) + "_" + std::string(#VALUE_NAME),  EnumName::VALUE_NAME)
#define add_widget_base(Name) add_type<Name>(#Name)
#define add_signal_emitter_base(Name) add_type<Name>(#Name)

// SignalEmitter
make_not_mirrored(AbstractSignalEmitter)

template<typename T, typename Arg_t>
void make_signal_emitter(Arg_t& type, jlcxx::Module& module)
{
    type
        .add_type_method(T, set_signal_blocked)
        .add_type_method(T, get_all_signal_names)
        //.add_type_method(T, connect_signal)
        .add_type_method(T, disconnect_signal)
        .add_type_method(T, new_signal)
    ;
}

// Vector2f

jl_value_t* wrap_vector2f(Vector2f in)
{
    auto* new_vector2f = jl_get_function((jl_module_t*) jl_get_global(jl_main_module, jl_symbol("mousetrap")), "Vector2f");

    if (new_vector2f == nullptr)
        log::fatal("In get_size_request: Unable to retrieve Julia function pointer to Vector2f(x, y)");

    auto* out = jl_call2(new_vector2f, jl_box_float32(in.x), jl_box_float32(in.y));
    return out == nullptr ? jl_nothing : out;
}

// Widget
make_not_mirrored(AbstractWidget)

template<typename T, typename Arg_t>
void make_widget(Arg_t& type, jlcxx::Module& module)
{
    type
        .add_type_method(T, set_margin_top)
        .add_type_method(T, get_margin_top)
        .add_type_method(T, set_margin_bottom)
        .add_type_method(T, set_margin_start)
        .add_type_method(T, get_margin_start)
        .add_type_method(T, set_margin_end)
        .add_type_method(T, get_margin_end)
        .add_type_method(T, set_margin_horizontal)
        .add_type_method(T, set_margin_vertical)
        .add_type_method(T, set_margin)
    ;

    module.method("set_size_request", [](T& in, float width, float height) -> void {
       in.Widget::set_size_request({width, height});
    });

    module.method("get_size_request", [](T& in) -> jl_value_t* {
        return wrap_vector2f(in.Widget::get_size_request());
    });

    module.set_override_module(jl_base_module);
    module.method("==", [](const T& a, const T& b){ return a == b; });
    module.method("!=", [](const T& a, const T& b){ return a == b; });
    module.unset_override_module();

    // TODO realize
    // TODO unrealize
    // TODO destroy
    // TODO hide
    // TODO show
    // TODO map
    // TODO unmap

    make_signal_emitter<T>(type, module);
}

// Application
void add_application(jlcxx::Module& module)
{
    auto application = module.add_signal_emitter_base(Application)
        .constructor<const std::string&>()
        .add_type_method(Application, run)
        .add_type_method(Application, quit)
        .add_type_method(Application, hold)
        .add_type_method(Application, release)
        .add_type_method(Application, mark_as_busy)
        .add_type_method(Application, unmark_as_busy)
        .add_type_method(Application, add_action)
        .add_type_method(Application, remove_action)
        .add_type_method(Application, get_action)
        .add_type_method(Application, has_action)
        //TODO .add_type_method(Application, set_menubar)
        .add_type_method(Application, get_id)
    ;
    make_signal_emitter<Application>(application, module);

    module.method("connect_signal_activate", [](Application& instance, jl_function_t* julia_function){

        if (not jl_isa(julia_function, (jl_value_t*) jl_function_type))
            log::critical("In Application::connect_signal_activate: Argument #1 is not a function", MOUSETRAP_DOMAIN);

        instance.connect_signal_activate([f = julia_function](Application* instance) {
            jl_safe_call(f, jlcxx::box<Application&>(*instance).value);
        });
    });

    // TODO activate
    // TODO shutdown
}

declare_is_subtype_of(Application, AbstractSignalEmitter)

// Window
void add_window(jlcxx::Module& module)
{
    auto window = module.add_widget_base(Window)
        .constructor<>()
        .constructor<Application&>()
        .add_type_method(Window, set_application)
        .add_type_method(Window, set_maximized)
        .add_type_method(Window, set_fullscreen)
        .add_type_method(Window, present)
        .add_type_method(Window, set_show_menubar)
        .add_type_method(Window, set_hide_on_close)
        .add_type_method(Window, close)
        .add_type_method(Window, set_child)
        .add_type_method(Window, get_child)
        .add_type_method(Window, set_destroy_with_parent)
        .add_type_method(Window, get_destroy_with_parent)
        .add_type_method(Window, set_title)
        .add_type_method(Window, get_title)
        .add_type_method(Window, set_titlebar_widget)
        .add_type_method(Window, get_titlebar_widget)
        .add_type_method(Window, set_is_modal)
        .add_type_method(Window, get_is_modal)
        .add_type_method(Window, set_transient_for)
        .add_type_method(Window, set_is_decorated)
        .add_type_method(Window, get_is_decorated)
        .add_type_method(Window, set_has_close_button)
        .add_type_method(Window, get_has_close_button)
        .add_type_method(Window, set_startup_notification_identifier)
        .add_type_method(Window, set_focus_visible)
        .add_type_method(Window, get_focus_visible)
    ;
    make_widget<Window>(window, module);

    // TODO close_request
    // TODO activate_default_widget
    // TODO activate_focused_widget
}

declare_is_subtype_of(Window, AbstractWidget)

// Action
void add_action(jlcxx::Module& module)
{
    auto action = module.add_type<Action>("Action")
        .constructor<std::string>(true)
        .add_type_method(Action, get_id)
        .add_type_method(Action, set_state)
        .add_type_method(Action, get_state)
        .add_type_method(Action, activate)
        .add_type_method(Action, set_enabled)
        .add_type_method(Action, get_enabled)
        .add_type_method(Action, get_is_stateful)
        .add_type_method(Action, add_shortcut)
    ;

    module.method("set_function", [](Action& action, jl_function_t* julia_function){
        // TODO
    });

    module.method("set_stateful_function", [](Action& action, jl_function_t* julia_function, bool initial_state)
    {
        // TODO
    });
}

// Log
make_not_mirrored(mousetrap::log)
void add_log(jlcxx::Module& module)
{
    module.set_const("MOUSETRAP_DOMAIN", std::string(MOUSETRAP_DOMAIN));

    module.add_enum(LogLevel);
    module.add_enum_value(LogLevel, LOG_LEVEL, FATAL);
    module.add_enum_value(LogLevel, LOG_LEVEL, CRITICAL);
    module.add_enum_value(LogLevel, LOG_LEVEL, WARNING);
    module.add_enum_value(LogLevel, LOG_LEVEL, INFO);
    module.add_enum_value(LogLevel, LOG_LEVEL, DEBUG);

    module.add_type<mousetrap::log>("log");
    #define add_log_function(name) module.method(std::string("log_") + #name, &mousetrap::log::name);
    add_log_function(initialize);
    add_log_function(debug);
    add_log_function(info);
    add_log_function(warning);
    add_log_function(critical);
    add_log_function(fatal);
    add_log_function(set_surpress_debug);
    add_log_function(set_surpress_info);
    add_log_function(get_surpress_info);
    add_log_function(get_surpress_debug);
    add_log_function(set_file);
    add_log_function(reset_file_formatting_function);
}

// Adjustment
void add_adjustment(jlcxx::Module& module)
{
    module.add_signal_emitter_base(Adjustment)
        .constructor<float, float, float, float>()
        .add_type_method(Adjustment, get_lower)
        .add_type_method(Adjustment, get_upper)
        .add_type_method(Adjustment, get_value)
        .add_type_method(Adjustment, get_increment)
        .add_type_method(Adjustment, set_lower)
        .add_type_method(Adjustment, set_upper)
        .add_type_method(Adjustment, set_value)
        .add_type_method(Adjustment, set_increment)
    ;

    // TODO value_changed
    // TODO properties_changed
}

declare_is_subtype_of(Adjustment, AbstractSignalEmitter);

// Alignment
void add_alignment(jlcxx::Module& module)
{
    module.add_enum(Alignment);
    module.add_enum_value(Alignment, ALIGNMENT, START);
    module.add_enum_value(Alignment, ALIGNMENT, CENTER);
    module.add_enum_value(Alignment, ALIGNMENT, END);
}

// Angle
void add_angle(jlcxx::Module& module)
{
    module.add_type<Angle>("Angle")
        .constructor<float>()
        .add_type_method(Angle, as_degrees)
        .add_type_method(Angle, as_radians)
    ;

    module.method("degrees", mousetrap::degrees);
    module.method("radians", mousetrap::radians);

    module.set_override_module(jl_base_module);
    module.method("==", [](Angle& a, Angle& b){ return a == b; });
    module.method("!=", [](Angle& a, Angle& b){ return a != b; });
    module.method("+", [](Angle& a, Angle& b){ return a + b; });
    module.method("-", [](Angle& a, Angle& b){ return a + b; });
    module.method("*", [](Angle& a, Angle& b){ return a - b; });
    module.method("/", [](Angle& a, Angle& b){ return a / b; });
    module.unset_override_module();
}

// AspectFrame
void add_aspect_frame(jlcxx::Module& module)
{
    auto aspect_frame = module.add_widget_base(AspectFrame)
        .constructor<float>()
        .add_type_method(AspectFrame, set_ratio)
        .add_type_method(AspectFrame, get_ratio)
        .add_type_method(AspectFrame, set_child_x_alignment)
        .add_type_method(AspectFrame, set_child_y_alignment)
        .add_type_method(AspectFrame, get_child_x_alignment)
        .add_type_method(AspectFrame, get_child_y_alignment)
        .add_type_method(AspectFrame, set_child)
        .add_type_method(AspectFrame, remove_child)
    ;
    
    make_widget<AspectFrame>(aspect_frame, module);
}

declare_is_subtype_of(AspectFrame, AbstractWidget)

// BlendMode
void add_blend_mode(jlcxx::Module& module)
{
    module.add_enum(BlendMode);
    module.add_enum_value(BlendMode, BLEND_MODE, NONE);
    module.add_enum_value(BlendMode, BLEND_MODE, NORMAL);
    module.add_enum_value(BlendMode, BLEND_MODE, ADD);
    module.add_enum_value(BlendMode, BLEND_MODE, SUBTRACT);
    module.add_enum_value(BlendMode, BLEND_MODE, REVERSE_SUBTRACT);
    module.add_enum_value(BlendMode, BLEND_MODE, MULTIPLY);
    module.add_enum_value(BlendMode, BLEND_MODE, MIN);
    module.add_enum_value(BlendMode, BLEND_MODE, MAX);
    
    module.add_global_method(set_current_blend_mode);
    module.add_global_method(blend_mode_to_string);
    module.add_global_method(blend_mode_from_string);
}

// Orientation
void add_orientation(jlcxx::Module& module)
{
    module.add_enum(Orientation);
    module.add_enum_value(Orientation, ORIENTATION, HORIZONTAL);
    module.add_enum_value(Orientation, ORIENTATION, VERTICAL);
}

// Box
void add_box(jlcxx::Module& module)
{
    auto box = module.add_widget_base(Box)
        .constructor<Orientation>()
        .add_type_method(Box, push_back)
        .add_type_method(Box, push_front)
        .add_type_method(Box, insert_after)
        .add_type_method(Box, remove)
        .add_type_method(Box, clear)
        .add_type_method(Box, set_homogeneous)
        .add_type_method(Box, get_homogeneous)
        .add_type_method(Box, set_spacing)
        .add_type_method(Box, get_spacing)
        .add_type_method(Box, get_n_items)
        .add_type_method(Box, get_orientation)
        .add_type_method(Box, set_orientation)
    ;

    make_widget<Box>(box, module);
}

declare_is_subtype_of(Box, AbstractWidget)

// Button
void add_button(jlcxx::Module& module)
{
    auto button = module.add_widget_base(Button)
        .constructor<>()
        .add_type_method(Button, set_has_frame)
        .add_type_method(Button, get_has_frame)
        .add_type_method(Button, set_child)
        .add_type_method(Button, get_child)
        .add_type_method(Button, set_action)
    ;

    make_widget<Button>(button, module);

    // TODO activate
    // TODO clicked
}

declare_is_subtype_of(Button, AbstractWidget)

// CenterBox
void add_center_box(jlcxx::Module& module)
{
    auto center_box = module.add_widget_base(CenterBox)
        .constructor<Orientation>()
        .add_type_method(CenterBox, set_start_widget)
        .add_type_method(CenterBox, get_start_widget)
        .add_type_method(CenterBox, set_center_widget)
        .add_type_method(CenterBox, get_center_widget)
        .add_type_method(CenterBox, set_end_widget)
        .add_type_method(CenterBox, get_end_widget)
        .add_type_method(CenterBox, set_orientation)
        .add_type_method(CenterBox, get_orientation)
    ;
    make_widget<CenterBox>(center_box, module);
}

declare_is_subtype_of(CenterBox, AbstractWidget)

// CheckButton
void add_check_button(jlcxx::Module& module)
{
    module.add_enum(CheckButtonState);
    module.add_enum_value(CheckButtonState, CHECK_BUTTON_STATE, ACTIVE);
    module.add_enum_value(CheckButtonState, CHECK_BUTTON_STATE, INACTIVE);
    module.add_enum_value(CheckButtonState, CHECK_BUTTON_STATE, INCONSISTENT);

    auto check_button = module.add_widget_base(CheckButton)
        .constructor<>()
        .add_type_method(CheckButton, set_state)
        .add_type_method(CheckButton, get_state)
        .add_type_method(CheckButton, set_child)
        .add_type_method(CheckButton, get_child)
    ;
    make_widget<CheckButton>(check_button, module);

    // TODO activate
    // TODO toggled
}

declare_is_subtype_of(CheckButton, AbstractWidget)

// Colors
void add_colors(jlcxx::Module& module)
{
    // TODO
}

// Event Controller

#define add_event_controller_base(Name) add_type<Name>(#Name)

void add_event_controllers(jlcxx::Module& module)
{
    module.add_enum(PropagationPhase);
    module.add_enum_value(PropagationPhase, PROPAGATION_PHASE, NONE);
    module.add_enum_value(PropagationPhase, PROPAGATION_PHASE, CAPTURE);
    module.add_enum_value(PropagationPhase, PROPAGATION_PHASE, BUBBLE);
    module.add_enum_value(PropagationPhase, PROPAGATION_PHASE, TARGET);

    module.add_type<EventController>("EventController");
    module.add_event_controller_base(ClickEventController);
    // TODO click_pressed
    // TODO click_released
    // TODO click_stopped

    module.add_event_controller_base(DragEventController)
        .constructor()
    ;

    module.method("get_start_position", [](DragEventController& in){
        return wrap_vector2f(in.get_start_position());
    });

    module.method("get_current_offset", [](DragEventController& in){
        return wrap_vector2f(in.get_current_offset());
    });

    // TODO drag_begin
    // TODO drag
    // TODO drag_end

    module.add_event_controller_base(FocusEventController)
        .constructor()
        .add_type_method(FocusEventController, self_or_child_is_focused)
        .add_type_method(FocusEventController, self_is_focused)
    ;
    // TODO focus_gained
    // TODO focus_lost

    module.add_event_controller_base(KeyEventController)
        .constructor()
        .add_type_method(KeyEventController, should_shortcut_trigger_trigger)
    ;
    // TODO key_pressed
    // TODO key_released
    // TODO modifiers_changed

    module.add_event_controller_base(MotionEventController)
        .constructor()
    ;
    /// TODO motion_enter
    /// TODO motion
    /// TODO motion_leave

    module.add_event_controller_base(PinchZoomEventController)
        .constructor()
        .add_type_method(PinchZoomEventController, get_scale_delta)
    ;
    // TODO scale changed

    module.add_event_controller_base(RotateEventController)
        .constructor()
        .add_type_method(RotateEventController, get_angle_delta)
    ;
    // TODO rotation_changed

    module.add_event_controller_base(ScrollEventController)
        .constructor<bool, bool>()
    ;
    // TODO scroll_begin
    // TODO scroll
    // TODO scroll_end
    // TODO kinetic_scroll_decelerate
}

declare_is_subtype_of(ClickEventController, EventController)
declare_is_subtype_of(DragEventController, EventController)
declare_is_subtype_of(FocusEventController, EventController)
declare_is_subtype_of(KeyEventController, EventController)
declare_is_subtype_of(MotionEventController, EventController)
declare_is_subtype_of(PinchZoomEventController, EventController)
declare_is_subtype_of(RotateEventController, EventController)
declare_is_subtype_of(ScrollEventController, EventController)

// SelectionModel


JLCXX_MODULE define_julia_module(jlcxx::Module& module)
{
    module.add_type<AbstractSignalEmitter>("AbstractSignalEmitter");
    module.add_type<SignalEmitter>("SignalEmitter");

    module.add_type<AbstractWidget>("AbstractWidget");
    module.add_type<Widget>("Widget");

    module.method("test", [](){
        return std::vector<size_t>();
    });

    // order matters

    add_angle(module);
    add_log(module);
    add_action(module);
    add_application(module);
    add_event_controllers(module);
    add_window(module);
    add_adjustment(module);
    add_alignment(module);
    add_aspect_frame(module);
    add_blend_mode(module);
    add_orientation(module);
    add_box(module);
    add_button(module);
    add_center_box(module);
    add_colors(module);

    // TODO clipboard
}
