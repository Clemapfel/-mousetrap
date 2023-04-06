//
// Created by clem on 3/31/23.
//

#include <mousetrap.hpp>
#include <jlcxx/jlcxx.hpp>

using namespace mousetrap;

#define add_method(Type, id) method(#id, &Type::id)
#define declare_is_subtype_of(Sub, Super) template<> struct jlcxx::SuperType<Sub> { typedef Super type; };

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

declare_is_subtype_of(Widget, SignalEmitter)

// Application
void add_application(jlcxx::Module& module)
{
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
}

declare_is_subtype_of(Application, SignalEmitter)

// Window
void add_window(jlcxx::Module& module)
{
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

declare_is_subtype_of(Window, Widget)

// Vector
/*
template<typename T> struct number_to_string {};
template<> struct number_to_string<float> { static inline const std::string name = "f"; };
template<> struct number_to_string<int64_t> { static inline const std::string name = "i"; };
template<> struct number_to_string<uint64_t> { static inline const std::string name = "ui"; };

template<> struct jlcxx::IsMirroredType<Vector2f> : std::false_type { };

template<typename T>
void add_vector2(jlcxx::Module& module)
{
    std::stringstream name;
    name << "Vector2" << number_to_string<T>::name;
    module.add_type<glm::vec<2, T>>(name.str())
        .method("x", [](Vector2<T> in) -> T {return in.x;})
        .method("y", [](Vector2<T> in) -> T {return in.y;})
    ;
}

/*
template<typename T>
void add_vector3(jlcxx::Module& module)
{
    std::stringstream name;
    name << "Vector2" << number_to_string<T>::name;
    module.add_type<glm::vec<3, T>>(name.str())
        .method("x", &glm::vec<3, T>::x)
        .method("y", &glm::vec<3, T>::y)
        .method("z", &glm::vec<3, T>::z)
        .method("r", &glm::vec<3, T>::r)
        .method("g", &glm::vec<3, T>::g)
        .method("b", &glm::vec<3, T>::b)
    ;
}

template<typename T>
void add_vector4(jlcxx::Module& module)
{
    std::stringstream name;
    name << "Vector2" << number_to_string<T>::name;
    module.add_type<glm::vec<4, T>>(name.str())
        .method("x", &glm::vec<4, T>::x)
        .method("y", &glm::vec<4, T>::y)
        .method("z", &glm::vec<4, T>::z)
        .method("w", &glm::vec<4, T>::w)
        .method("r", &glm::vec<4, T>::r)
        .method("g", &glm::vec<4, T>::g)
        .method("b", &glm::vec<4, T>::b)
        .method("a", &glm::vec<4, T>::a)
    ;
}
 */

JLCXX_MODULE define_julia_module(jlcxx::Module& module)
{
    module.add_type<Widget>("Widget");
    module.add_type<SignalEmitter>("SignalEmitter");

    module.add_type<ApplicationID>("ApplicationID").constructor<std::string>();

    add_application(module);
    add_window(module);
}
