//
// Copyright (c) Clemens Cords (mail@clemens-cords.com), created 3/17/23
//

#pragma once

#include <functional>
#include <include/gtk_common.hpp>

// TODO
#include <iostream>

namespace mousetrap
{
    /// @brief Parent class of all has_signal_<signal_id> components
    struct SignalComponent {};

    #define SPLAT(...) __VA_ARGS__

    #define CTOR_SIGNAL(T, signal_name) \
        SIGNAL_CLASS_NAME(signal_name)<T>(this)

    #define HAS_SIGNAL(T, signal_name) \
        public SIGNAL_CLASS_NAME(signal_name)<T>

    #define SIGNAL_INTERNAL_PRIVATE_CLASS_NAME(CamelCase) _##CamelCase
    #define SIGNAL_INTERNAL_CLASS_NAME(CamelCase) CamelCase
    #define SIGNAL_CLASS_NAME(snake_case) has_signal_##snake_case

    #define DECLARE_SIGNAL(CamelCase, snake_case, CAPS_CASE, g_signal_id, return_t) \
    \
    namespace detail \
    { \
        struct SIGNAL_INTERNAL_PRIVATE_CLASS_NAME(CamelCase) \
        { \
            GObject parent; \
            void* instance; \
            std::function<return_t(void*)> function; \
            bool blocked; \
        }; \
        using SIGNAL_INTERNAL_CLASS_NAME(CamelCase) = SIGNAL_INTERNAL_PRIVATE_CLASS_NAME(CamelCase); \
        SIGNAL_INTERNAL_CLASS_NAME(CamelCase)* snake_case##_new(void* instance); \
    } \
    \
    template<typename T> \
    class SIGNAL_CLASS_NAME(snake_case) \
    { \
        private: \
            detail::SIGNAL_INTERNAL_CLASS_NAME(CamelCase)* _internal = nullptr;               \
            T* _instance = nullptr;                                                                        \
            \
            static return_t wrapper(void*, detail::SIGNAL_INTERNAL_CLASS_NAME(CamelCase)* internal) \
            { \
                if (not internal->blocked and internal->function) \
                    return internal->function(internal->instance); \
                else \
                    return return_t(); \
            }                                                                       \
                                                                                    \
            void initialize()                                                       \
            {                                                                       \
                if (_internal == nullptr)                                           \
                {                                                                    \
                    _internal = detail::snake_case##_new((void*) _instance); \
                    detail::attach_ref_to(_instance->operator GObject*(), _internal);  \
                    g_object_ref(_internal);                                                                    \
                }                                                                        \
            }\
        \
        protected: \
            SIGNAL_CLASS_NAME(snake_case)(T* instance) \
                : _instance(instance) \
            {} \
                                                                                    \
            ~SIGNAL_CLASS_NAME(snake_case)()                                        \
            {                                      \
                if (_internal != nullptr)                                           \
                    g_object_unref(_internal);\
            }\
        \
        public: \
            static inline constexpr const char* signal_id = g_signal_id; \
            \
            template<typename Function_t, typename Data_t> \
            void connect_signal_##snake_case(Function_t f, Data_t data) \
            { \
                initialize();                                                                    \
                _internal->function = [f, data](void* instance) -> return_t { \
                    return f((T*) instance, data); \
                }; \
                ((T*) _internal->instance)->connect_signal(signal_id, wrapper, _internal); \
            } \
            \
            template<typename Function_t> \
            void connect_signal_##snake_case(Function_t f) \
            { \
                initialize();                                                                    \
                _internal->function = [f](void* instance) -> return_t { \
                    return f((T*) instance); \
                }; \
                ((T*) _internal->instance)->connect_signal(signal_id, wrapper, _internal); \
            } \
            \
            void set_signal_##snake_case##_blocked(bool b) \
            { \
                initialize();                                                                    \
                _internal->blocked = b; \
            } \
            \
            bool get_signal_##snake_case##_blocked() const \
            { \
                initialize();                                                                    \
                return _internal->blocked; \
            } \
            \
            return_t emit_signal_##snake_case() \
            { \
                initialize();                                                                    \
                return wrapper(nullptr, _internal); \
            } \
            \
            void disconnect_signal_##snake_case() \
            { \
                initialize();                                                                    \
                ((T*) _internal->instance)->disconnect_signal(signal_id); \
            } \
    };

    /// @brief declare a signal with the signature (T* instance, arg_list..., auto data) -> return_t
    /// @param signal_name name of the signal, has to be a valid C++ variable name
    /// @param g_signal_id Gio ID of the signal, string
    /// @param return_t return type of the signal wrapper function
    /// @param arg_list list of argument types an names, for example `float x, float y`
    /// @param arg_name_list list of arguments **without** the type, for example `x, y`
    #define DECLARE_SIGNAL_MANUAL(signal_name, g_signal_id, return_t, arg_list, arg_name_list)        \
        template<typename T>                                                                          \
        class SIGNAL_CLASS_NAME(signal_name) : protected SignalComponent                                                             \
        {                                                                                             \
            private:                                                                                  \
                T* _instance = nullptr;                                                               \
                std::function<return_t(T* instance, arg_list)> _function = nullptr;                             \
                bool _blocked = false;                                                                \
                                                                                                      \
                static return_t wrapper(void*, arg_list, SIGNAL_CLASS_NAME(signal_name)<T>* self)         \
                {                                                                                     \
                    return self->emit_signal_##signal_name(arg_name_list);                                        \
                }                                                                                     \
                                                                                                      \
            protected:                                                                                \
                explicit SIGNAL_CLASS_NAME(signal_name)(T* instance)                                      \
                    : _instance(instance)                                                             \
                {}                                                                                    \
                                                                                                      \
            public:                                                                                   \
                static inline constexpr const char* signal_id = g_signal_id;                          \
                                                                                                      \
                SIGNAL_CLASS_NAME(signal_name)(const SIGNAL_CLASS_NAME(signal_name)<T>& other) = delete;      \
                SIGNAL_CLASS_NAME(signal_name)<T>& operator=(const SIGNAL_CLASS_NAME(signal_name)<T>& other) = delete;  \
                                                                                                   \
                SIGNAL_CLASS_NAME(signal_name)(SIGNAL_CLASS_NAME(signal_name)<T>&& other)          \
                {         \
                    _function = other._function;                                                   \
                    other._function = nullptr;                                                                                  \
                    _blocked = other._blocked; \
                }                                                                                  \
                SIGNAL_CLASS_NAME(signal_name)<T>& operator=(SIGNAL_CLASS_NAME(signal_name)<T>&& other)\
                {                                                                                  \
                    _function = other._function; \
                    _blocked = other._blocked;                                          \
                    return *this;                                                                               \
                }                                                                                      \
                                                                                                      \
                template<typename Data_t> \
                using signal_handler_with_data_f = std::function<return_t(T* instance, arg_list, Data_t data)>; \
                                                                                                      \
                template<typename Function_t, typename Data_t>                                        \
                void connect_signal_##signal_name(Function_t function, Data_t data)                   \
                {                                                                                     \
                    _function = [f = function, d = data](T* instance, arg_list)                       \
                    {                                                                                 \
                        return f(instance, arg_name_list, d);                                            \
                    };                                                                                \
                    ((T*) _instance)->connect_signal(signal_id, wrapper, this); \
                }                                                                                     \
                                                                                              \
                using signal_handler_without_data_f = std::function<return_t(T* instance, arg_list)>;                                                                                      \
                                                                                                      \
                void connect_signal_##signal_name(const signal_handler_without_data_f& function)                                \
                {                                                                                     \
                    _function = [f = function](T* instance, arg_list)                                 \
                    {                                                                                 \
                        return f(instance, arg_name_list);                                            \
                    };                                                                                \
                    ((T*) _instance)->connect_signal(signal_id, wrapper, this); \
                }                                                                                     \
                                                                                                      \
                void set_signal_##signal_name##_blocked(bool b)                                       \
                {                                                                                     \
                    _blocked = b;                                                                     \
                }                                                                                     \
                                                                                                      \
                bool get_signal_##signal_name##_blocked() const                                       \
                {                                                                                     \
                    return _blocked;                                                                  \
                }                                                                                     \
                                                                                                      \
                return_t emit_signal_##signal_name(arg_list)                                                    \
                {                                                                                     \
                    if (not _blocked and _function != nullptr)                                                                 \
                        return _function(_instance, arg_name_list);                                                  \
                    else                                                                              \
                        return return_t();                                                         \
                }                                                                                     \
                                                                                                      \
                void disconnect_signal_##signal_name()                                                \
                {                                                                                     \
                    _instance->disconnect_signal(signal_id);                                      \
                }                                                                                  \
        }

    /// @see
    DECLARE_SIGNAL(Activate, activate, ACTIVATE, "activate", void);
    /// @class has_signal_activate
    /// @brief signal emitted when activate
    /// @tparam T instance type
    ///
    /// @fn void has_signal_activate::emit_signal_activate()
    /// \signal_emit_brief
    ///
    /// @var has_signal_activate::signal_id
    /// \signal_id{activate}
    ///
    /// @var has_signal_activate::signal_handler_with_data_f
    /// \signal_with_data_f{activate}
    ///
    /// @var has_signal_activate::signal_handler_without_data_f
    /// \signal_without_data_f{activate}
    ///
    /// @fn void has_signal_activate::connect_signal_activate(const signal_handler_with_data_f<Data_t>& function, Data_t data)
    /// \signal_connect_data
    ///
    /// @fn void has_signal_activate::connect_signal_activate(const signal_handler_without_data_f&)
    /// \signal_connect_no_data
    ///
    /// @fn void has_signal_activate::set_signal_activate_blocked(bool)
    /// \signal_set_blocked
    ///
    /// @fn bool has_signal_activate::get_signal_activate_blocked() const
    /// \signal_get_blocked
    ///
    /// @fn void has_signal_activate::disconnect_signal_activate()
    /// \signal_disconnect
    ///
    /// @fn has_signal_activate::has_signal_activate
    /// \signal_ctor

    DECLARE_SIGNAL(Startup, startup, STARTUP, "startup", void);
    /// @class has_signal_startup
    /// @brief signal emitted when application registers with the OS, this usually happens before `activate` is emitted
    /// @tparam T instance type
    ///
    /// @fn void has_signal_startup::emit_signal_startup()
    /// \signal_emit_brief
    ///
    /// @var has_signal_startup::signal_id
    /// \signal_id{https://docs.gtk.org/gio/signal.Application.startup.html}
    ///
    /// @var has_signal_startup::signal_handler_with_data_f
    /// \signal_with_data_f{startup}
    ///
    /// @var has_signal_startup::signal_handler_without_data_f
    /// \signal_without_data_f{startup}
    ///
    /// @fn void has_signal_startup::connect_signal_startup(const signal_handler_with_data_f<Data_t>& function, Data_t data)
    /// \signal_connect_data
    ///
    /// @fn void has_signal_startup::connect_signal_startup(const signal_handler_without_data_f&)
    /// \signal_connect_no_data
    ///
    /// @fn void has_signal_startup::set_signal_startup_blocked(bool)
    /// \signal_set_blocked
    ///
    /// @fn bool has_signal_startup::get_signal_startup_blocked() const
    /// \signal_get_blocked
    ///
    /// @fn void has_signal_startup::disconnect_signal_startup()
    /// \signal_disconnect
    ///
    /// @fn has_signal_startup::has_signal_startup
    /// \signal_ctor

    DECLARE_SIGNAL(Shutdown, shutdown, SHUTDOWN, "shutdown", void);
    /// @class has_signal_shutdown
    /// @brief signal emitted when application runtime ends
    /// @tparam T instance type
    ///
    /// @fn void has_signal_shutdown::emit_signal_shutdown()
    /// \signal_emit_brief
    ///
    /// @var has_signal_shutdown::signal_id
    /// \signal_id{https://docs.gtk.org/gio/signal.Application.startup.html}
    ///
    /// @var has_signal_shutdown::signal_handler_with_data_f
    /// \signal_with_data_f{shutdown}
    ///
    /// @var has_signal_shutdown::signal_handler_without_data_f
    /// \signal_without_data_f{shutdown}
    ///
    /// @fn void has_signal_shutdown::connect_signal_shutdown(const signal_handler_with_data_f<Data_t>& function, Data_t data)
    /// \signal_connect_data
    ///
    /// @fn void has_signal_shutdown::connect_signal_shutdown(const signal_handler_without_data_f&)
    /// \signal_connect_no_data
    ///
    /// @fn void has_signal_shutdown::set_signal_shutdown_blocked(bool)
    /// \signal_set_blocked
    ///
    /// @fn bool has_signal_shutdown::get_signal_shutdown_blocked() const
    /// \signal_get_blocked
    ///
    /// @fn void has_signal_shutdown::disconnect_signal_shutdown()
    /// \signal_disconnect
    ///
    /// @fn has_signal_shutdown::has_signal_shutdown
    /// \signal_ctor

    DECLARE_SIGNAL(Update, update, UPDATE, "update", void);
    /// @class has_signal_update
    /// @brief signal emitted when a frame clock is updated, this happens exactly once per render frame
    /// @tparam T instance type
    ///
    /// @fn void has_signal_update::emit_signal_update()
    /// \signal_emit_brief
    ///
    /// @var has_signal_update::signal_id
    /// \signal_id{https://docs.gtk.org/gdk4/signal.FrameClock.update.html}
    ///
    /// @var has_signal_update::signal_handler_with_data_f
    /// \signal_with_data_f{update}
    ///
    /// @var has_signal_update::signal_handler_without_data_f
    /// \signal_without_data_f{update}
    ///
    /// @fn void has_signal_update::connect_signal_update(const signal_handler_with_data_f<Data_t>& function, Data_t data)
    /// \signal_connect_data
    ///
    /// @fn void has_signal_update::connect_signal_update(const signal_handler_without_data_f&)
    /// \signal_connect_no_data
    ///
    /// @fn void has_signal_update::set_signal_update_blocked(bool)
    /// \signal_set_blocked
    ///
    /// @fn bool has_signal_update::get_signal_update_blocked() const
    /// \signal_get_blocked
    ///
    /// @fn void has_signal_update::disconnect_signal_update()
    /// \signal_disconnect
    ///
    /// @fn has_signal_update::has_signal_update
    /// \signal_ctor

    DECLARE_SIGNAL(Paint, paint, PAINT, "paint", void);
    /// @class has_signal_paint
    /// @brief signal emitted during the render step of the frame
    /// @tparam T instance type
    ///
    /// @fn void has_signal_paint::emit_signal_paint()
    /// \signal_emit_brief
    ///
    /// @var has_signal_paint::signal_id
    /// \signal_id{https://docs.gtk.org/gdk4/signal.FrameClock.paint.html}
    ///
    /// @var has_signal_paint::signal_handler_with_data_f
    /// \signal_with_data_f{paint}
    ///
    /// @var has_signal_paint::signal_handler_without_data_f
    /// \signal_without_data_f{paint}
    ///
    /// @fn void has_signal_paint::connect_signal_paint(const signal_handler_with_data_f<Data_t>& function, Data_t data)
    /// \signal_connect_data
    ///
    /// @fn void has_signal_paint::connect_signal_paint(const signal_handler_without_data_f&)
    /// \signal_connect_no_data
    ///
    /// @fn void has_signal_paint::set_signal_paint_blocked(bool)
    /// \signal_set_blocked
    ///
    /// @fn bool has_signal_paint::get_signal_paint_blocked() const
    /// \signal_get_blocked
    ///
    /// @fn void has_signal_paint::disconnect_signal_paint()
    /// \signal_disconnect
    ///
    /// @fn has_signal_paint::has_signal_paint
    /// \signal_ctor

    DECLARE_SIGNAL(Realize, realize, REALIZE, "realize", void);
    /// @class has_signal_realize
    /// @brief signal emitted when a widget is realized
    /// @tparam T instance type
    ///
    /// @fn void has_signal_realize::emit_signal_realize()
    /// \signal_emit_brief
    ///
    /// @var has_signal_realize::signal_id
    /// \signal_id{https://docs.gtk.org/gtk4/signal.Widget.realize.html}
    ///
    /// @var has_signal_realize::signal_handler_with_data_f
    /// \signal_with_data_f{realize}
    ///
    /// @var has_signal_realize::signal_handler_without_data_f
    /// \signal_without_data_f{realize}
    ///
    /// @fn void has_signal_realize::connect_signal_realize(const signal_handler_with_data_f<Data_t>& function, Data_t data)
    /// \signal_connect_data
    ///
    /// @fn void has_signal_realize::connect_signal_realize(const signal_handler_without_data_f&)
    /// \signal_connect_no_data
    ///
    /// @fn void has_signal_realize::set_signal_realize_blocked(bool)
    /// \signal_set_blocked
    ///
    /// @fn bool has_signal_realize::get_signal_realize_blocked() const
    /// \signal_get_blocked
    ///
    /// @fn void has_signal_realize::disconnect_signal_realize()
    /// \signal_disconnect
    ///
    /// @fn has_signal_realize::has_signal_realize
    /// \signal_ctor

    DECLARE_SIGNAL(Unrealize, unrealize, UNREALIZE, "unrealize", void);
    /// @class has_signal_unrealize
    /// @brief signal emitted when a widget seizes to be displayed, it will deallocate its size and stop rendering
    /// @tparam T instance type
    ///
    /// @fn void has_signal_unrealize::emit_signal_unrealize()
    /// \signal_emit_brief
    ///
    /// @var has_signal_unrealize::signal_id
    /// \signal_id{https://docs.gtk.org/gtk4/signal.Widget.unrealize.html}
    ///
    /// @var has_signal_unrealize::signal_handler_with_data_f
    /// \signal_with_data_f{unrealize}
    ///
    /// @var has_signal_unrealize::signal_handler_without_data_f
    /// \signal_without_data_f{unrealize}
    ///
    /// @fn void has_signal_unrealize::connect_signal_unrealize(const signal_handler_with_data_f<Data_t>& function, Data_t data)
    /// \signal_connect_data
    ///
    /// @fn void has_signal_unrealize::connect_signal_unrealize(const signal_handler_without_data_f&)
    /// \signal_connect_no_data
    ///
    /// @fn void has_signal_unrealize::set_signal_unrealize_blocked(bool)
    /// \signal_set_blocked
    ///
    /// @fn bool has_signal_unrealize::get_signal_unrealize_blocked() const
    /// \signal_get_blocked
    ///
    /// @fn void has_signal_unrealize::disconnect_signal_unrealize()
    /// \signal_disconnect
    ///
    /// @fn has_signal_unrealize::has_signal_unrealize
    /// \signal_ctor

    DECLARE_SIGNAL(Destroy, destroy, DESTROY, "destroy", void);
    /// @class has_signal_destroy
    /// @brief signal emitted when a widget is destroyed, this happens only once the reference count of a widget reaches 0, calling Widget::~Widget may not necessarily emit this signal
    /// @tparam T instance type
    ///
    /// @fn void has_signal_destroy::emit_signal_destroy()
    /// \signal_emit_brief
    ///
    /// @var has_signal_destroy::signal_id
    /// \signal_id{https://docs.gtk.org/gtk4/signal.Widget.destroy.html}
    ///
    /// @var has_signal_destroy::signal_handler_with_data_f
    /// \signal_with_data_f{destroy}
    ///
    /// @var has_signal_destroy::signal_handler_without_data_f
    /// \signal_without_data_f{destroy}
    ///
    /// @fn void has_signal_destroy::connect_signal_destroy(const signal_handler_with_data_f<Data_t>& function, Data_t data)
    /// \signal_connect_data
    ///
    /// @fn void has_signal_destroy::connect_signal_destroy(const signal_handler_without_data_f&)
    /// \signal_connect_no_data
    ///
    /// @fn void has_signal_destroy::set_signal_destroy_blocked(bool)
    /// \signal_set_blocked
    ///
    /// @fn bool has_signal_destroy::get_signal_destroy_blocked() const
    /// \signal_get_blocked
    ///
    /// @fn void has_signal_destroy::disconnect_signal_destroy()
    /// \signal_disconnect
    ///
    /// @fn has_signal_destroy::has_signal_destroy
    /// \signal_ctor

    DECLARE_SIGNAL(Hide, hide, HIDE, "hide", void);
    /// @class has_signal_hide
    /// @brief signal emitted when a widget is hidden, usually triggered by calling Widget::hide
    /// @tparam T instance type
    ///
    /// @fn void has_signal_hide::emit_signal_hide()
    /// \signal_emit_brief
    ///
    /// @var has_signal_hide::signal_id
    /// \signal_id{https://docs.gtk.org/gtk4/signal.Widget.hide.html}
    ///
    /// @var has_signal_hide::signal_handler_with_data_f
    /// \signal_with_data_f{hide}
    ///
    /// @var has_signal_hide::signal_handler_without_data_f
    /// \signal_without_data_f{hide}
    ///
    /// @fn void has_signal_hide::connect_signal_hide(const signal_handler_with_data_f<Data_t>& function, Data_t data)
    /// \signal_connect_data
    ///
    /// @fn void has_signal_hide::connect_signal_hide(const signal_handler_without_data_f&)
    /// \signal_connect_no_data
    ///
    /// @fn void has_signal_hide::set_signal_hide_blocked(bool)
    /// \signal_set_blocked
    ///
    /// @fn bool has_signal_hide::get_signal_hide_blocked() const
    /// \signal_get_blocked
    ///
    /// @fn void has_signal_hide::disconnect_signal_hide()
    /// \signal_disconnect
    ///
    /// @fn has_signal_hide::has_signal_hide
    /// \signal_ctor

    DECLARE_SIGNAL(Show, show, SHOW, "show", void);
    /// @class has_signal_show
    /// @brief signal emitted when a widget is shown or presented
    /// @tparam T instance type
    ///
    /// @fn void has_signal_show::emit_signal_show()
    /// \signal_emit_brief
    ///
    /// @var has_signal_show::signal_id
    /// \signal_id{https://docs.gtk.org/gtk4/signal.Widget.show.html}
    ///
    /// @var has_signal_show::signal_handler_with_data_f
    /// \signal_with_data_f{show}
    ///
    /// @var has_signal_show::signal_handler_without_data_f
    /// \signal_without_data_f{show}
    ///
    /// @fn void has_signal_show::connect_signal_show(const signal_handler_with_data_f<Data_t>& function, Data_t data)
    /// \signal_connect_data
    ///
    /// @fn void has_signal_show::connect_signal_show(const signal_handler_without_data_f&)
    /// \signal_connect_no_data
    ///
    /// @fn void has_signal_show::set_signal_show_blocked(bool)
    /// \signal_set_blocked
    ///
    /// @fn bool has_signal_show::get_signal_show_blocked() const
    /// \signal_get_blocked
    ///
    /// @fn void has_signal_show::disconnect_signal_show()
    /// \signal_disconnect
    ///
    /// @fn has_signal_show::has_signal_show
    /// \signal_ctor

    DECLARE_SIGNAL(Map, map, MAP, "map", void);
    /// @class has_signal_map
    /// @brief signal emitted when a widget is mapped, this happens when it and all of its parents become visible
    /// @tparam T instance type
    ///
    /// @fn void has_signal_map::emit_signal_map()
    /// \signal_emit_brief
    ///
    /// @var has_signal_map::signal_id
    /// \signal_id{https://docs.gtk.org/gtk4/signal.Widget.map.html}
    ///
    /// @var has_signal_map::signal_handler_with_data_f
    /// \signal_with_data_f{map}
    ///
    /// @var has_signal_map::signal_handler_without_data_f
    /// \signal_without_data_f{map}
    ///
    /// @fn void has_signal_map::connect_signal_map(const signal_handler_with_data_f<Data_t>& function, Data_t data)
    /// \signal_connect_data
    ///
    /// @fn void has_signal_map::connect_signal_map(const signal_handler_without_data_f&)
    /// \signal_connect_no_data
    ///
    /// @fn void has_signal_map::set_signal_map_blocked(bool)
    /// \signal_set_blocked
    ///
    /// @fn bool has_signal_map::get_signal_map_blocked() const
    /// \signal_get_blocked
    ///
    /// @fn void has_signal_map::disconnect_signal_map()
    /// \signal_disconnect
    ///
    /// @fn has_signal_map::has_signal_map
    /// \signal_ctor

    DECLARE_SIGNAL(Unmap, unmap, UNMAP, "unmap", void);
    /// @class has_signal_unmap
    /// @brief signal emitted when a widget is unmapped, this happens when it is currently visible and it or at least one of its parents becomes invisible
    /// @tparam T instance type
    ///
    /// @fn void has_signal_unmap::emit_signal_unmap()
    /// \signal_emit_brief
    ///
    /// @var has_signal_unmap::signal_id
    /// \signal_id{https://docs.gtk.org/gtk4/signal.Widget.unmap.html}
    ///
    /// @var has_signal_unmap::signal_handler_with_data_f
    /// \signal_with_data_f{unmap}
    ///
    /// @var has_signal_unmap::signal_handler_without_data_f
    /// \signal_without_data_f{unmap}
    ///
    /// @fn void has_signal_unmap::connect_signal_unmap(const signal_handler_with_data_f<Data_t>& function, Data_t data)
    /// \signal_connect_data
    ///
    /// @fn void has_signal_unmap::connect_signal_unmap(const signal_handler_without_data_f&)
    /// \signal_connect_no_data
    ///
    /// @fn void has_signal_unmap::set_signal_unmap_blocked(bool)
    /// \signal_set_blocked
    ///
    /// @fn bool has_signal_unmap::get_signal_unmap_blocked() const
    /// \signal_get_blocked
    ///
    /// @fn void has_signal_unmap::disconnect_signal_unmap()
    /// \signal_disconnect
    ///
    /// @fn has_signal_unmap::has_signal_unmap
    /// \signal_ctor

    DECLARE_SIGNAL(CloseRequest, close_request, CLOSE_REQUEST, "close-request", bool);
    /// @class has_signal_close_request
    /// @brief signal emitted when a window is requested to close, usually by calling Window::close or when a user presses the `x` button in the windows title bar
    /// @tparam T instance type
    ///
    /// @fn void has_signal_close_request::emit_signal_close_request()
    /// \signal_emit_brief
    ///
    /// @var has_signal_close_request::signal_id
    /// \signal_id{https://docs.gtk.org/gtk4/signal.Window.close-request.html}
    ///
    /// @var has_signal_close_request::signal_handler_with_data_f
    /// \signal_with_data_f{close_request}
    ///
    /// @var has_signal_close_request::signal_handler_without_data_f
    /// \signal_without_data_f{close_request}
    ///
    /// @fn void has_signal_close_request::connect_signal_close_request(const signal_handler_with_data_f<Data_t>& function, Data_t data)
    /// \signal_connect_data
    ///
    /// @fn void has_signal_close_request::connect_signal_close_request(const signal_handler_without_data_f&)
    /// \signal_connect_no_data
    ///
    /// @fn void has_signal_close_request::set_signal_close_request_blocked(bool)
    /// \signal_set_blocked
    ///
    /// @fn bool has_signal_close_request::get_signal_close_request_blocked() const
    /// \signal_get_blocked
    ///
    /// @fn void has_signal_close_request::disconnect_signal_close_request()
    /// \signal_disconnect
    ///
    /// @fn has_signal_close_request::has_signal_close_request
    /// \signal_ctor

    DECLARE_SIGNAL(ActivateDefaultWidget, activate_default_widget, ACTIVATE_DEFAULT_WIDGET, "activate-default", void);
    /// @class has_signal_activate_default_widget
    /// @brief signal emitted when the default widget of a window is activated
    /// @tparam T instance type
    ///
    /// @fn void has_signal_activate_default_widget::emit_signal_activate_default_widget()
    /// \signal_emit_brief
    ///
    /// @var has_signal_activate_default_widget::signal_id
    /// \signal_id{https://docs.gtk.org/gtk4/signal.Window.activate-default.html}
    ///
    /// @var has_signal_activate_default_widget::signal_handler_with_data_f
    /// \signal_with_data_f{activate_default_widget}
    ///
    /// @var has_signal_activate_default_widget::signal_handler_without_data_f
    /// \signal_without_data_f{activate_default_widget}
    ///
    /// @fn void has_signal_activate_default_widget::connect_signal_activate_default_widget(const signal_handler_with_data_f<Data_t>& function, Data_t data)
    /// \signal_connect_data
    ///
    /// @fn void has_signal_activate_default_widget::connect_signal_activate_default_widget(const signal_handler_without_data_f&)
    /// \signal_connect_no_data
    ///
    /// @fn void has_signal_activate_default_widget::set_signal_activate_default_widget_blocked(bool)
    /// \signal_set_blocked
    ///
    /// @fn bool has_signal_activate_default_widget::get_signal_activate_default_widget_blocked() const
    /// \signal_get_blocked
    ///
    /// @fn void has_signal_activate_default_widget::disconnect_signal_activate_default_widget()
    /// \signal_disconnect
    ///
    /// @fn has_signal_activate_default_widget::has_signal_activate_default_widget
    /// \signal_ctor

    DECLARE_SIGNAL(ActivateFocusWidget, activate_focused_widget, ACTIVATE_FOCUS_WIDGET, "activate-focus", void);
    /// @class has_signal_activate_focused_widget
    /// @brief signal emitted when the focused widget of a window is activated
    /// @tparam T instance type
    ///
    /// @fn void has_signal_activate_focused_widget::emit_signal_activate_focused_widget()
    /// \signal_emit_brief
    ///
    /// @var has_signal_activate_focused_widget::signal_id
    /// \signal_id{https://docs.gtk.org/gtk4/signal.Window.activate-focus.html}
    ///
    /// @var has_signal_activate_focused_widget::signal_handler_with_data_f
    /// \signal_with_data_f{activate_focused_widget}
    ///
    /// @var has_signal_activate_focused_widget::signal_handler_without_data_f
    /// \signal_without_data_f{activate_focused_widget}
    ///
    /// @fn void has_signal_activate_focused_widget::connect_signal_activate_focused_widget(const signal_handler_with_data_f<Data_t>& function, Data_t data)
    /// \signal_connect_data
    ///
    /// @fn void has_signal_activate_focused_widget::connect_signal_activate_focused_widget(const signal_handler_without_data_f&)
    /// \signal_connect_no_data
    ///
    /// @fn void has_signal_activate_focused_widget::set_signal_activate_focused_widget_blocked(bool)
    /// \signal_set_blocked
    ///
    /// @fn bool has_signal_activate_focused_widget::get_signal_activate_focused_widget_blocked() const
    /// \signal_get_blocked
    ///
    /// @fn void has_signal_activate_focused_widget::disconnect_signal_activate_focused_widget()
    /// \signal_disconnect
    ///
    /// @fn has_signal_activate_focused_widget::has_signal_activate_focused_widget
    /// \signal_ctor

    DECLARE_SIGNAL(Clicked, clicked, CLICKED, "clicked", void);
    /// @class has_signal_clicked
    /// @brief signal emitted when a button is clicked by the user, this is different from activating the button
    /// @tparam T instance type
    ///
    /// @fn void has_signal_clicked::emit_signal_clicked()
    /// \signal_emit_brief
    ///
    /// @var has_signal_clicked::signal_id
    /// \signal_id{https://docs.gtk.org/gtk4/signal.Button.activate.html}
    ///
    /// @var has_signal_clicked::signal_handler_with_data_f
    /// \signal_with_data_f{clicked}
    ///
    /// @var has_signal_clicked::signal_handler_without_data_f
    /// \signal_without_data_f{clicked}
    ///
    /// @fn void has_signal_clicked::connect_signal_clicked(const signal_handler_with_data_f<Data_t>& function, Data_t data)
    /// \signal_connect_data
    ///
    /// @fn void has_signal_clicked::connect_signal_clicked(const signal_handler_without_data_f&)
    /// \signal_connect_no_data
    ///
    /// @fn void has_signal_clicked::set_signal_clicked_blocked(bool)
    /// \signal_set_blocked
    ///
    /// @fn bool has_signal_clicked::get_signal_clicked_blocked() const
    /// \signal_get_blocked
    ///
    /// @fn void has_signal_clicked::disconnect_signal_clicked()
    /// \signal_disconnect
    ///
    /// @fn has_signal_clicked::has_signal_clicked
    /// \signal_ctor

    DECLARE_SIGNAL(Toggled, toggled, TOGGLED, "toggled", void);
    /// @class has_signal_toggled
    /// @brief signal emitted when a stateful-button such as CheckButton or ToggleButton changes its state
    /// @tparam T instance type
    ///
    /// @fn void has_signal_toggled::emit_signal_toggled()
    /// \signal_emit_brief
    ///
    /// @var has_signal_toggled::signal_id
    /// \signal_id{https://docs.gtk.org/gtk4/signal.ToggleButton.toggled.html}
    ///
    /// @var has_signal_toggled::signal_handler_with_data_f
    /// \signal_with_data_f{toggled}
    ///
    /// @var has_signal_toggled::signal_handler_without_data_f
    /// \signal_without_data_f{toggled}
    ///
    /// @fn void has_signal_toggled::connect_signal_toggled(const signal_handler_with_data_f<Data_t>& function, Data_t data)
    /// \signal_connect_data
    ///
    /// @fn void has_signal_toggled::connect_signal_toggled(const signal_handler_without_data_f&)
    /// \signal_connect_no_data
    ///
    /// @fn void has_signal_toggled::set_signal_toggled_blocked(bool)
    /// \signal_set_blocked
    ///
    /// @fn bool has_signal_toggled::get_signal_toggled_blocked() const
    /// \signal_get_blocked
    ///
    /// @fn void has_signal_toggled::disconnect_signal_toggled()
    /// \signal_disconnect
    ///
    /// @fn has_signal_toggled::has_signal_toggled
    /// \signal_ctor

    DECLARE_SIGNAL(TextChanged, text_changed, TEXT_CHANGED, "changed", void);
    /// @class has_signal_text_changed
    /// @brief signal emitted when the text buffer of an editable widget changes
    /// @tparam T instance type
    ///
    /// @fn void has_signal_text_changed::emit_signal_text_changed()
    /// \signal_emit_brief
    ///
    /// @var has_signal_text_changed::signal_id
    /// \signal_id{https://docs.gtk.org/gtk4/signal.Editable.changed.html}
    ///
    /// @var has_signal_text_changed::signal_handler_with_data_f
    /// \signal_with_data_f{text_changed}
    ///
    /// @var has_signal_text_changed::signal_handler_without_data_f
    /// \signal_without_data_f{text_changed}
    ///
    /// @fn void has_signal_text_changed::connect_signal_text_changed(const signal_handler_with_data_f<Data_t>& function, Data_t data)
    /// \signal_connect_data
    ///
    /// @fn void has_signal_text_changed::connect_signal_text_changed(const signal_handler_without_data_f&)
    /// \signal_connect_no_data
    ///
    /// @fn void has_signal_text_changed::set_signal_text_changed_blocked(bool)
    /// \signal_set_blocked
    ///
    /// @fn bool has_signal_text_changed::get_signal_text_changed_blocked() const
    /// \signal_get_blocked
    ///
    /// @fn void has_signal_text_changed::disconnect_signal_text_changed()
    /// \signal_disconnect
    ///
    /// @fn has_signal_text_changed::has_signal_text_changed
    /// \signal_ctor

    DECLARE_SIGNAL_MANUAL(text_inserted, "insert-text", void,
          SPLAT(int32_t start_pos, int32_t end_pos),
          SPLAT(start_pos, end_pos)
    );
    /// @class has_signal_text_inserted
    /// @brief signal emitted when text is inserted
    /// @tparam T instance type
    ///
    /// @fn void has_signal_text_inserted::emit_signal_text_inserted(int32_t start_pos, int32_t end_pos)
    /// \signal_emit_brief
    /// @param start_pos index of the first letter of the inserted text
    /// @param end_pos index after the last letter of the inserted text
    ///
    /// @var has_signal_text_inserted::signal_id
    /// \signal_id{https://docs.gtk.org/gtk4/signal.Editable.insert-text.html}
    ///
    /// @var has_signal_text_inserted::signal_handler_with_data_f
    /// \signal_with_data_f{text_inserted}
    /// \signal_see_emit_for_parameters{text_inserted}
    ///
    /// @var has_signal_text_inserted::signal_handler_without_data_f
    /// \signal_without_data_f{text_inserted}
    /// \signal_see_emit_for_parameters{text_inserted}
    ///
    /// @fn void has_signal_text_inserted::connect_signal_text_inserted(const signal_handler_with_data_f<Data_t>& function, Data_t data)
    /// \signal_connect_data
    /// \signal_see_emit_for_parameters{text_inserted}
    ///
    /// @fn void has_signal_text_inserted::connect_signal_text_inserted(const signal_handler_without_data_f&)
    /// \signal_connect_no_data
    /// \signal_see_emit_for_parameters{text_inserted}
    ///
    /// @fn void has_signal_text_inserted::set_signal_text_inserted_blocked(bool)
    /// \signal_set_blocked
    ///
    /// @fn bool has_signal_text_inserted::get_signal_text_inserted_blocked() const
    /// \signal_get_blocked
    ///
    /// @fn void has_signal_text_inserted::disconnect_signal_text_inserted()
    /// \signal_disconnect
    ///
    /// @fn has_signal_text_inserted::has_signal_text_inserted
    /// \signal_ctor

    DECLARE_SIGNAL_MANUAL(text_deleted, "delete-text", void,
          SPLAT(int32_t start_pos, int32_t end_pos),
          SPLAT(start_pos, end_pos)
    );
    /// @class has_signal_text_deleted
    /// @brief signal emitted when text is deleted from a text buffer
    /// @tparam T instance type
    ///
    /// @fn void has_signal_text_deleted::emit_signal_text_deleted(int32_t start_pos, int32_t end_pos)
    /// \signal_emit_brief
    /// @param start_pos index of the first letter of the deleted text
    /// @param end_pos index after the last letter of the deleted text
    ///
    /// @var has_signal_text_deleted::signal_id
    /// \signal_id{https://docs.gtk.org/gtk4/signal.Editable.delete-text.html}
    ///
    /// @var has_signal_text_deleted::signal_handler_with_data_f
    /// \signal_with_data_f{text_deleted}
    /// \signal_see_emit_for_parameters{text_deleted}
    ///
    /// @var has_signal_text_deleted::signal_handler_without_data_f
    /// \signal_without_data_f{text_deleted}
    /// \signal_see_emit_for_parameters{text_deleted}
    ///
    /// @fn void has_signal_text_deleted::connect_signal_text_deleted(const signal_handler_with_data_f<Data_t>& function, Data_t data)
    /// \signal_connect_data
    /// \signal_see_emit_for_parameters{text_inserted}
    ///
    /// @fn void has_signal_text_deleted::connect_signal_text_deleted(const signal_handler_without_data_f&)
    /// \signal_connect_no_data
    /// \signal_see_emit_for_parameters{text_inserted}
    ///
    /// @fn void has_signal_text_deleted::set_signal_text_deleted_blocked(bool)
    /// \signal_set_blocked
    ///
    /// @fn bool has_signal_text_deleted::get_signal_text_deleted_blocked() const
    /// \signal_get_blocked
    ///
    /// @fn void has_signal_text_deleted::disconnect_signal_text_deleted()
    /// \signal_disconnect
    ///
    /// @fn has_signal_text_deleted::has_signal_text_deleted
    /// \signal_ctor

    DECLARE_SIGNAL(Undo, undo, UNDO, "undo", void);
    /// @class has_signal_undo
    /// @brief signal emitted when the user or application request an `undo` action for a text buffer
    /// @tparam T instance type
    ///
    /// @fn void has_signal_undo::emit_signal_undo()
    /// \signal_emit_brief
    ///
    /// @var has_signal_undo::signal_id
    /// \signal_id{https://docs.gtk.org/gtk4/signal.TextBuffer.undo.html}
    ///
    /// @var has_signal_undo::signal_handler_with_data_f
    /// \signal_with_data_f{undo}
    ///
    /// @var has_signal_undo::signal_handler_without_data_f
    /// \signal_without_data_f{undo}
    ///
    /// @fn void has_signal_undo::connect_signal_undo(const signal_handler_with_data_f<Data_t>& function, Data_t data)
    /// \signal_connect_data
    ///
    /// @fn void has_signal_undo::connect_signal_undo(const signal_handler_without_data_f&)
    /// \signal_connect_no_data
    ///
    /// @fn void has_signal_undo::set_signal_undo_blocked(bool)
    /// \signal_set_blocked
    ///
    /// @fn bool has_signal_undo::get_signal_undo_blocked() const
    /// \signal_get_blocked
    ///
    /// @fn void has_signal_undo::disconnect_signal_undo()
    /// \signal_disconnect
    ///
    /// @fn has_signal_undo::has_signal_undo
    /// \signal_ctor

    DECLARE_SIGNAL(Redo, redo, REDO, "redo", void);
    /// @class has_signal_redo
    /// @brief signal emitted when the user or application requests a `redo` action for a text buffer
    /// @tparam T instance type
    ///
    /// @fn void has_signal_redo::emit_signal_redo()
    /// \signal_emit_brief
    ///
    /// @var has_signal_redo::signal_id
    /// \signal_id{https://docs.gtk.org/gtk4/signal.TextBuffer.redo.html}
    ///
    /// @var has_signal_redo::signal_handler_with_data_f
    /// \signal_with_data_f{redo}
    ///
    /// @var has_signal_redo::signal_handler_without_data_f
    /// \signal_without_data_f{redo}
    ///
    /// @fn void has_signal_redo::connect_signal_redo(const signal_handler_with_data_f<Data_t>& function, Data_t data)
    /// \signal_connect_data
    ///
    /// @fn void has_signal_redo::connect_signal_redo(const signal_handler_without_data_f&)
    /// \signal_connect_no_data
    ///
    /// @fn void has_signal_redo::set_signal_redo_blocked(bool)
    /// \signal_set_blocked
    ///
    /// @fn bool has_signal_redo::get_signal_redo_blocked() const
    /// \signal_get_blocked
    ///
    /// @fn void has_signal_redo::disconnect_signal_redo()
    /// \signal_disconnect
    ///
    /// @fn has_signal_redo::has_signal_redo
    /// \signal_ctor

    DECLARE_SIGNAL_MANUAL(selection_changed, "selection-changed", void,
        SPLAT(int32_t position, int32_t n_items),
        SPLAT(position, n_items)
    );
    /// @class has_signal_selection_changed
    /// @brief signal emitted when selection of a widget managed by a mousetrap::SelectionModel changes
    /// @tparam T instance type
    ///
    /// @fn void has_signal_selection_changed::emit_signal_selection_changed(int32_t position, int32_t n_items)
    /// \signal_emit_brief
    /// @param position index of the element that changed
    /// @param n_items number of items that changed
    ///
    /// @var has_signal_selection_changed::signal_id
    /// \signal_id{https://docs.gtk.org/gtk4/signal.SelectionModel.selection-changed.html}
    ///
    /// @var has_signal_selection_changed::signal_handler_with_data_f
    /// \signal_with_data_f{selection_changed}
    /// \signal_see_emit_for_parameters{selection_changed}
    ///
    /// @var has_signal_selection_changed::signal_handler_without_data_f
    /// \signal_without_data_f{selection_changed}
    /// \signal_see_emit_for_parameters{selection_changed}
    ///
    /// @fn void has_signal_selection_changed::connect_signal_selection_changed(const signal_handler_with_data_f<Data_t>& function, Data_t data)
    /// \signal_connect_data
    /// \signal_see_emit_for_parameters{text_inserted}
    ///
    /// @fn void has_signal_selection_changed::connect_signal_selection_changed(const signal_handler_without_data_f&)
    /// \signal_connect_no_data
    /// \signal_see_emit_for_parameters{text_inserted}
    ///
    /// @fn void has_signal_selection_changed::set_signal_selection_changed_blocked(bool)
    /// \signal_set_blocked
    ///
    /// @fn bool has_signal_selection_changed::get_signal_selection_changed_blocked() const
    /// \signal_get_blocked
    ///
    /// @fn void has_signal_selection_changed::disconnect_signal_selection_changed()
    /// \signal_disconnect
    ///
    /// @fn has_signal_selection_changed::has_signal_selection_changed
    /// \signal_ctor

    using ModifierState = GdkModifierType;
    using KeyValue = guint;
    using KeyCode = guint;

    DECLARE_SIGNAL_MANUAL(key_pressed, "key-pressed", bool,
        SPLAT(KeyValue keyval, KeyCode keycode, ModifierState modifier),
        SPLAT(keyval, keycode, modifier)
    );
    /// @class has_signal_key_pressed
    /// @brief signal emitted when a widget that has a mousetrap::KeyEventController connected captures a keypress event, usually from the user pressing a keyboard key
    /// @tparam T instance type
    ///
    /// @fn void has_signal_key_pressed::emit_signal_key_pressed(KeyValue keyval, KeyCode keycode, ModifierState modifier)
    /// \signal_emit_brief
    /// @param keyval key identifier, a complete list of keys can be found at https://gitlab.gnome.org/GNOME/gtk/-/blob/main/gdk/gdkkeysyms.h
    /// @param keycode os-supplied internal identifier, can usually be ignored. Test against keyval instead
    /// @param modifier modifier state, can be queried to see whether Alt, Control, Shift, etc. are held down during this key event
    ///
    /// @var has_signal_key_pressed::signal_id
    /// \signal_id{https://docs.gtk.org/gtk4/signal.EventControllerKey.key-pressed.html}
    ///
    /// @var has_signal_key_pressed::signal_handler_with_data_f
    /// \signal_with_data_f{key_pressed}
    /// \signal_see_emit_for_parameters{key_pressed}
    ///
    /// @var has_signal_key_pressed::signal_handler_without_data_f
    /// \signal_without_data_f{key_pressed}
    /// \signal_see_emit_for_parameters{key_pressed}
    ///
    /// @fn void has_signal_key_pressed::connect_signal_key_pressed(const signal_handler_with_data_f<Data_t>& function, Data_t data)
    /// \signal_connect_data
    /// \signal_see_emit_for_parameters{text_inserted}
    ///
    /// @fn void has_signal_key_pressed::connect_signal_key_pressed(const signal_handler_without_data_f&)
    /// \signal_connect_no_data
    /// \signal_see_emit_for_parameters{text_inserted}
    ///
    /// @fn void has_signal_key_pressed::set_signal_key_pressed_blocked(bool)
    /// \signal_set_blocked
    ///
    /// @fn bool has_signal_key_pressed::get_signal_key_pressed_blocked() const
    /// \signal_get_blocked
    ///
    /// @fn void has_signal_key_pressed::disconnect_signal_key_pressed()
    /// \signal_disconnect
    ///
    /// @fn has_signal_key_pressed::has_signal_key_pressed
    /// \signal_ctor

    DECLARE_SIGNAL_MANUAL(key_released, "key-released", void,
        SPLAT(KeyValue keyval, KeyCode keycode, ModifierState modifier),
        SPLAT(keyval, keycode, modifier)
    );
    /// @class has_signal_key_released
    /// @brief signal emitted when a widget that has a mousetrap::KeyEventController connected captures a key release event, usually from the user seizing to press a keyboard key
    /// @tparam T instance type
    ///
    /// @fn void has_signal_key_released::emit_signal_key_released(KeyValue keyval, KeyCode keycode, ModifierState modifier)
    /// \signal_emit_brief
    /// @param keyval key identifier, a complete list of keys can be found at https://gitlab.gnome.org/GNOME/gtk/-/blob/main/gdk/gdkkeysyms.h
    /// @param keycode os-supplied internal identifier, can usually be ignored. Test against keyval instead
    /// @param modifier modifier state, can be queried to see whether Alt, Control, Shift, etc. are held down during this key event
    ///
    /// @var has_signal_key_released::signal_id
    /// \signal_id{https://docs.gtk.org/gtk4/signal.EventControllerKey.key-released.html}
    ///
    /// @var has_signal_key_released::signal_handler_with_data_f
    /// \signal_with_data_f{key_released}
    /// \signal_see_emit_for_parameters{key_released}
    ///
    /// @var has_signal_key_released::signal_handler_without_data_f
    /// \signal_without_data_f{key_released}
    /// \signal_see_emit_for_parameters{key_released}
    ///
    /// @fn void has_signal_key_released::connect_signal_key_released(const signal_handler_with_data_f<Data_t>& function, Data_t data)
    /// \signal_connect_data
    /// \signal_see_emit_for_parameters{text_inserted}
    ///
    /// @fn void has_signal_key_released::connect_signal_key_released(const signal_handler_without_data_f&)
    /// \signal_connect_no_data
    /// \signal_see_emit_for_parameters{text_inserted}
    ///
    /// @fn void has_signal_key_released::set_signal_key_released_blocked(bool)
    /// \signal_set_blocked
    ///
    /// @fn bool has_signal_key_released::get_signal_key_released_blocked() const
    /// \signal_get_blocked
    ///
    /// @fn void has_signal_key_released::disconnect_signal_key_released()
    /// \signal_disconnect
    ///
    /// @fn has_signal_key_released::has_signal_key_released
    /// \signal_ctor

    /// @see https://docs.gtk.org/gtk4/signal.EventControllerKey.key-pressed.html
    DECLARE_SIGNAL_MANUAL(modifiers_changed, "modifiers", bool,
        SPLAT(KeyValue keyval, KeyCode keycode, ModifierState modifier),
        SPLAT(keyval, keycode, modifier)
    );
    /// @class has_signal_modifiers_changed
    /// @brief signal emitted when modifiers keys (Control, Alt, Shift, etc.) change
    /// @tparam T instance type
    ///
    /// @fn void has_signal_modifiers_changed::emit_signal_modifiers_changed(KeyValue keyval, KeyCode keycode, ModifierState modifier)
    /// \signal_emit_brief
    /// @param keyval key identifier, a complete list of keys can be found at https://gitlab.gnome.org/GNOME/gtk/-/blob/main/gdk/gdkkeysyms.h
    /// @param keycode os-supplied internal identifier, can usually be ignored. Test against keyval instead
    /// @param modifier modifier state, can be queried to see whether Alt, Control, Shift, etc. are held down during this key event
    ///
    /// @var has_signal_modifiers_changed::signal_id
    /// \signal_id{https://docs.gtk.org/gtk4/signal.EventControllerKey.modifiers.html}
    ///
    /// @var has_signal_modifiers_changed::signal_handler_with_data_f
    /// \signal_with_data_f{modifiers_changed}
    /// \signal_see_emit_for_parameters{modifiers_changed}
    ///
    /// @var has_signal_modifiers_changed::signal_handler_without_data_f
    /// \signal_without_data_f{modifiers_changed}
    /// \signal_see_emit_for_parameters{modifiers_changed}
    ///
    /// @fn void has_signal_modifiers_changed::connect_signal_modifiers_changed(const signal_handler_with_data_f<Data_t>& function, Data_t data)
    /// \signal_connect_data
    /// \signal_see_emit_for_parameters{text_inserted}
    ///
    /// @fn void has_signal_modifiers_changed::connect_signal_modifiers_changed(const signal_handler_without_data_f&)
    /// \signal_connect_no_data
    /// \signal_see_emit_for_parameters{text_inserted}
    ///
    /// @fn void has_signal_modifiers_changed::set_signal_modifiers_changed_blocked(bool)
    /// \signal_set_blocked
    ///
    /// @fn bool has_signal_modifiers_changed::get_signal_modifiers_changed_blocked() const
    /// \signal_get_blocked
    ///
    /// @fn void has_signal_modifiers_changed::disconnect_signal_modifiers_changed()
    /// \signal_disconnect
    ///
    /// @fn has_signal_modifiers_changed::has_signal_modifiers_changed
    /// \signal_ctor

    DECLARE_SIGNAL_MANUAL(motion_enter, "enter", void,
        SPLAT(double x, double y),
        SPLAT(x, y)
    );
    /// @class has_signal_motion_enter
    /// @brief signal emitted when cursor enters a widgets mapped area while it has a mousetrap::MotionEventController connected
    /// @tparam T instance type
    ///
    /// @fn void has_signal_motion_enter::emit_signal_motion_enter(double x, double y)
    /// \signal_emit_brief
    /// @param x x-coordinate of the cursor, widget-relative position, in pixels
    /// @param y y-coordinate of the cursor, widget-relative position, in pixels
    ///
    /// @var has_signal_motion_enter::signal_id
    /// \signal_id{https://docs.gtk.org/gtk4/signal.EventControllerMotion.enter.html}
    ///
    /// @var has_signal_motion_enter::signal_handler_with_data_f
    /// \signal_with_data_f{motion_enter}
    /// \signal_see_emit_for_parameters{motion_enter}
    ///
    /// @var has_signal_motion_enter::signal_handler_without_data_f
    /// \signal_without_data_f{motion_enter}
    /// \signal_see_emit_for_parameters{motion_enter}
    ///
    /// @fn void has_signal_motion_enter::connect_signal_motion_enter(const signal_handler_with_data_f<Data_t>& function, Data_t data)
    /// \signal_connect_data
    /// \signal_see_emit_for_parameters{text_inserted}
    ///
    /// @fn void has_signal_motion_enter::connect_signal_motion_enter(const signal_handler_without_data_f&)
    /// \signal_connect_no_data
    /// \signal_see_emit_for_parameters{text_inserted}
    ///
    /// @fn void has_signal_motion_enter::set_signal_motion_enter_blocked(bool)
    /// \signal_set_blocked
    ///
    /// @fn bool has_signal_motion_enter::get_signal_motion_enter_blocked() const
    /// \signal_get_blocked
    ///
    /// @fn void has_signal_motion_enter::disconnect_signal_motion_enter()
    /// \signal_disconnect
    ///
    /// @fn has_signal_motion_enter::has_signal_motion_enter
    /// \signal_ctor

    DECLARE_SIGNAL_MANUAL(motion, "motion", void,
        SPLAT(double x, double y),
        SPLAT(x, y)
    );
    /// @class has_signal_motion
    /// @brief signal emitted when cursor moves while inside the mapped area of a widget that has a mousetrap::MotionEventController connected
    /// @tparam T instance type
    ///
    /// @fn void has_signal_motion::emit_signal_motion(double x, double y)
    /// \signal_emit_brief
    /// @param x x-coordinate of the cursor, widget-relative position, in pixels
    /// @param y y-coordinate of the cursor, widget-relative position, in pixels
    ///
    /// @var has_signal_motion::signal_id
    /// \signal_id{https://docs.gtk.org/gtk4/signal.EventControllerMotion.motion.html}
    ///
    /// @var has_signal_motion::signal_handler_with_data_f
    /// \signal_with_data_f{motion}
    /// \signal_see_emit_for_parameters{motion}
    ///
    /// @var has_signal_motion::signal_handler_without_data_f
    /// \signal_without_data_f{motion}
    /// \signal_see_emit_for_parameters{motion}
    ///
    /// @fn void has_signal_motion::connect_signal_motion(const signal_handler_with_data_f<Data_t>& function, Data_t data)
    /// \signal_connect_data
    /// \signal_see_emit_for_parameters{text_inserted}
    ///
    /// @fn void has_signal_motion::connect_signal_motion(const signal_handler_without_data_f&)
    /// \signal_connect_no_data
    /// \signal_see_emit_for_parameters{text_inserted}
    ///
    /// @fn void has_signal_motion::set_signal_motion_blocked(bool)
    /// \signal_set_blocked
    ///
    /// @fn bool has_signal_motion::get_signal_motion_blocked() const
    /// \signal_get_blocked
    ///
    /// @fn void has_signal_motion::disconnect_signal_motion()
    /// \signal_disconnect
    ///
    /// @fn has_signal_motion::has_signal_motion
    /// \signal_ctor

    DECLARE_SIGNAL(MotionLeave, motion_leave, MOTION_LEAVE, "leave", void);
    /// @class has_signal_motion_leave
    /// @brief signal emitted when cursor exits the mapped area of a widget that has a mousetrap::MotionEventController connected
    /// @tparam T instance type
    ///
    /// @fn void has_signal_motion_leave::emit_signal_motion_leave()
    /// \signal_emit_brief
    ///
    /// @var has_signal_motion_leave::signal_id
    /// \signal_id{https://docs.gtk.org/gtk4/signal.EventControllerMotion.leave.html}
    ///
    /// @var has_signal_motion_leave::signal_handler_with_data_f
    /// \signal_with_data_f{motion_leave}
    ///
    /// @var has_signal_motion_leave::signal_handler_without_data_f
    /// \signal_without_data_f{motion_leave}
    ///
    /// @fn void has_signal_motion_leave::connect_signal_motion_leave(const signal_handler_with_data_f<Data_t>& function, Data_t data)
    /// \signal_connect_data
    ///
    /// @fn void has_signal_motion_leave::connect_signal_motion_leave(const signal_handler_without_data_f&)
    /// \signal_connect_no_data
    ///
    /// @fn void has_signal_motion_leave::set_signal_motion_leave_blocked(bool)
    /// \signal_set_blocked
    ///
    /// @fn bool has_signal_motion_leave::get_signal_motion_leave_blocked() const
    /// \signal_get_blocked
    ///
    /// @fn void has_signal_motion_leave::disconnect_signal_motion_leave()
    /// \signal_disconnect
    ///
    /// @fn has_signal_motion_leave::has_signal_motion_leave
    /// \signal_ctor

    DECLARE_SIGNAL_MANUAL(click_pressed, "pressed", void,
        SPLAT(int32_t n_press, double x, double y),
        SPLAT(n_press, x, y)
    );
    /// @class has_signal_click_pressed
    /// @brief signal emitted when user pressed one of the mouse buttons while inside a widgets mapped area that has a mousetrap::ClickEventController connected
    /// @tparam T instance type
    ///
    /// @fn void has_signal_click_pressed::emit_signal_click_pressed(int32_t n_press, double x, double y)
    /// \signal_emit_brief
    /// @param n_press number of clicks in this sequence
    /// @param x x-coordinate of the cursor, widget-relative position, in pixels
    /// @param y y-coordinate of the cursor, widget-relative position, in pixels
    ///
    /// @var has_signal_click_pressed::signal_id
    /// \signal_id{https://docs.gtk.org/gtk4/signal.GestureClick.pressed.html}
    ///
    /// @var has_signal_click_pressed::signal_handler_with_data_f
    /// \signal_with_data_f{click_pressed}
    /// \signal_see_emit_for_parameters{click_pressed}
    ///
    /// @var has_signal_click_pressed::signal_handler_without_data_f
    /// \signal_without_data_f{click_pressed}
    /// \signal_see_emit_for_parameters{click_pressed}
    ///
    /// @fn void has_signal_click_pressed::connect_signal_click_pressed(const signal_handler_with_data_f<Data_t>& function, Data_t data)
    /// \signal_connect_data
    /// \signal_see_emit_for_parameters{text_inserted}
    ///
    /// @fn void has_signal_click_pressed::connect_signal_click_pressed(const signal_handler_without_data_f&)
    /// \signal_connect_no_data
    /// \signal_see_emit_for_parameters{text_inserted}
    ///
    /// @fn void has_signal_click_pressed::set_signal_click_pressed_blocked(bool)
    /// \signal_set_blocked
    ///
    /// @fn bool has_signal_click_pressed::get_signal_click_pressed_blocked() const
    /// \signal_get_blocked
    ///
    /// @fn void has_signal_click_pressed::disconnect_signal_click_pressed()
    /// \signal_disconnect
    ///
    /// @fn has_signal_click_pressed::has_signal_click_pressed
    /// \signal_ctor

    DECLARE_SIGNAL_MANUAL(click_released, "released", void,
        SPLAT(int32_t n_press, double x, double y),
        SPLAT(n_press, x, y)
    );
    /// @class has_signal_click_released
    /// @brief signal emitted when user seizes to hold down one of the mouse buttons while inside a widgets mapped area that has a mousetrap::ClickEventController connected
    /// @tparam T instance type
    ///
    /// @fn void has_signal_click_released::emit_signal_click_released(int32_t n_press, double x, double y)
    /// \signal_emit_brief
    /// @param n_press number of clicks in this sequence
    /// @param x x-coordinate of the cursor, widget-relative position, in pixels
    /// @param y y-coordinate of the cursor, widget-relative position, in pixels
    ///
    /// @var has_signal_click_released::signal_id
    /// \signal_id{ https://docs.gtk.org/gtk4/signal.GestureClick.released.html}
    ///
    /// @var has_signal_click_released::signal_handler_with_data_f
    /// \signal_with_data_f{click_released}
    /// \signal_see_emit_for_parameters{click_released}
    ///
    /// @var has_signal_click_released::signal_handler_without_data_f
    /// \signal_without_data_f{click_released}
    /// \signal_see_emit_for_parameters{click_released}
    ///
    /// @fn void has_signal_click_released::connect_signal_click_released(const signal_handler_with_data_f<Data_t>& function, Data_t data)
    /// \signal_connect_data
    /// \signal_see_emit_for_parameters{text_inserted}
    ///
    /// @fn void has_signal_click_released::connect_signal_click_released(const signal_handler_without_data_f&)
    /// \signal_connect_no_data
    /// \signal_see_emit_for_parameters{text_inserted}
    ///
    /// @fn void has_signal_click_released::set_signal_click_released_blocked(bool)
    /// \signal_set_blocked
    ///
    /// @fn bool has_signal_click_released::get_signal_click_released_blocked() const
    /// \signal_get_blocked
    ///
    /// @fn void has_signal_click_released::disconnect_signal_click_released()
    /// \signal_disconnect
    ///
    /// @fn has_signal_click_released::has_signal_click_released
    /// \signal_ctor

    DECLARE_SIGNAL(ClickStopped, click_stopped, CLICK_STOPPED, "stopped", void);
    /// @class has_signal_click_stopped
    /// @brief signal emitted when a sequence of clicks stops, emitted by mousetrap::ClickEventController
    /// @tparam T instance type
    ///
    /// @fn void has_signal_click_stopped::emit_signal_click_stopped()
    /// \signal_emit_brief
    ///
    /// @var has_signal_click_stopped::signal_id
    /// \signal_id{https://docs.gtk.org/gtk4/signal.GestureClick.stopped.html}
    ///
    /// @var has_signal_click_stopped::signal_handler_with_data_f
    /// \signal_with_data_f{click_stopped}
    ///
    /// @var has_signal_click_stopped::signal_handler_without_data_f
    /// \signal_without_data_f{click_stopped}
    ///
    /// @fn void has_signal_click_stopped::connect_signal_click_stopped(const signal_handler_with_data_f<Data_t>& function, Data_t data)
    /// \signal_connect_data
    ///
    /// @fn void has_signal_click_stopped::connect_signal_click_stopped(const signal_handler_without_data_f&)
    /// \signal_connect_no_data
    ///
    /// @fn void has_signal_click_stopped::set_signal_click_stopped_blocked(bool)
    /// \signal_set_blocked
    ///
    /// @fn bool has_signal_click_stopped::get_signal_click_stopped_blocked() const
    /// \signal_get_blocked
    ///
    /// @fn void has_signal_click_stopped::disconnect_signal_click_stopped()
    /// \signal_disconnect
    ///
    /// @fn has_signal_click_stopped::has_signal_click_stopped
    /// \signal_ctor

    DECLARE_SIGNAL_MANUAL(kinetic_scroll_decelerate, "decelerate", void,
        SPLAT(double x_velocity, double y_velocity),
        SPLAT(x_velocity, y_velocity)
    );
    /// @class has_signal_kinetic_scroll_decelerate
    /// @brief signal emitted when kinetic scroll ends, this may be many frames after the user has already stopped pressing the scroll button
    /// @tparam T instance type
    ///
    /// @fn void has_signal_kinetic_scroll_decelerate::emit_signal_kinetic_scroll_decelerate(double x_velocity, double y_velocity)
    /// \signal_emit_brief
    /// @param x_velocity current x-velocity, if postive, scroll left to right, if negative, scrolls right to left
    /// @param y_velocity current y-velocity, if positive, scrolls top to bottom, if negative, scrolls bottom to top
    ///
    /// @var has_signal_kinetic_scroll_decelerate::signal_id
    /// \signal_id{https://docs.gtk.org/gtk4/signal.EventControllerScroll.decelerate.html}
    ///
    /// @var has_signal_kinetic_scroll_decelerate::signal_handler_with_data_f
    /// \signal_with_data_f{kinetic_scroll_decelerate}
    /// \signal_see_emit_for_parameters{kinetic_scroll_decelerate}
    ///
    /// @var has_signal_kinetic_scroll_decelerate::signal_handler_without_data_f
    /// \signal_without_data_f{kinetic_scroll_decelerate}
    /// \signal_see_emit_for_parameters{kinetic_scroll_decelerate}
    ///
    /// @fn void has_signal_kinetic_scroll_decelerate::connect_signal_kinetic_scroll_decelerate(const signal_handler_with_data_f<Data_t>& function, Data_t data)
    /// \signal_connect_data
    /// \signal_see_emit_for_parameters{text_inserted}
    ///
    /// @fn void has_signal_kinetic_scroll_decelerate::connect_signal_kinetic_scroll_decelerate(const signal_handler_without_data_f&)
    /// \signal_connect_no_data
    /// \signal_see_emit_for_parameters{text_inserted}
    ///
    /// @fn void has_signal_kinetic_scroll_decelerate::set_signal_kinetic_scroll_decelerate_blocked(bool)
    /// \signal_set_blocked
    ///
    /// @fn bool has_signal_kinetic_scroll_decelerate::get_signal_kinetic_scroll_decelerate_blocked() const
    /// \signal_get_blocked
    ///
    /// @fn void has_signal_kinetic_scroll_decelerate::disconnect_signal_kinetic_scroll_decelerate()
    /// \signal_disconnect
    ///
    /// @fn has_signal_kinetic_scroll_decelerate::has_signal_kinetic_scroll_decelerate
    /// \signal_ctor

    DECLARE_SIGNAL(ScrollBegin, scroll_begin, SCROLL_BEGIN, "scroll-begin", void);
    /// @class has_signal_scroll_begin
    /// @brief signal emitted the first frame a user starts a scrolling action, emitted by mousetrap::ScrollEventController
    /// @tparam T instance type
    ///
    /// @fn void has_signal_scroll_begin::emit_signal_scroll_begin()
    /// \signal_emit_brief
    ///
    /// @var has_signal_scroll_begin::signal_id
    /// \signal_id{https://docs.gtk.org/gtk4/signal.EventControllerScroll.scroll-begin.html}
    ///
    /// @var has_signal_scroll_begin::signal_handler_with_data_f
    /// \signal_with_data_f{scroll_begin}
    ///
    /// @var has_signal_scroll_begin::signal_handler_without_data_f
    /// \signal_without_data_f{scroll_begin}
    ///
    /// @fn void has_signal_scroll_begin::connect_signal_scroll_begin(const signal_handler_with_data_f<Data_t>& function, Data_t data)
    /// \signal_connect_data
    ///
    /// @fn void has_signal_scroll_begin::connect_signal_scroll_begin(const signal_handler_without_data_f&)
    /// \signal_connect_no_data
    ///
    /// @fn void has_signal_scroll_begin::set_signal_scroll_begin_blocked(bool)
    /// \signal_set_blocked
    ///
    /// @fn bool has_signal_scroll_begin::get_signal_scroll_begin_blocked() const
    /// \signal_get_blocked
    ///
    /// @fn void has_signal_scroll_begin::disconnect_signal_scroll_begin()
    /// \signal_disconnect
    ///
    /// @fn has_signal_scroll_begin::has_signal_scroll_begin
    /// \signal_ctor

    DECLARE_SIGNAL_MANUAL(scroll, "scroll", bool,
       SPLAT(double delta_x, double delta_y),
       SPLAT(delta_x, delta_y)
    );
    /// @class has_signal_scroll
    /// @brief signal emitted once per frame while the user scrolls a widget that has a mousetrap::ScrollEventController connected
    /// @tparam T instance type
    ///
    /// @fn void has_signal_scroll::emit_signal_scroll(double delta_x, double delta_y)
    /// \signal_emit_brief
    /// @param delta_x difference of horizontal scroll offsets between this and previous frame. If positive, scroll left to right, if negative, scroll right to left
    /// @param delta_y difference of vertical scroll offsets between this and previous frame. If positive, scroll top to bottom, if negative, scroll bottom to top
    ///
    /// @var has_signal_scroll::signal_id
    /// \signal_id{https://docs.gtk.org/gtk4/signal.EventControllerScroll.scroll.html}
    ///
    /// @var has_signal_scroll::signal_handler_with_data_f
    /// \signal_with_data_f{scroll}
    /// \signal_see_emit_for_parameters{scroll}
    ///
    /// @var has_signal_scroll::signal_handler_without_data_f
    /// \signal_without_data_f{scroll}
    /// \signal_see_emit_for_parameters{scroll}
    ///
    /// @fn void has_signal_scroll::connect_signal_scroll(const signal_handler_with_data_f<Data_t>& function, Data_t data)
    /// \signal_connect_data
    /// \signal_see_emit_for_parameters{text_inserted}
    ///
    /// @fn void has_signal_scroll::connect_signal_scroll(const signal_handler_without_data_f&)
    /// \signal_connect_no_data
    /// \signal_see_emit_for_parameters{text_inserted}
    ///
    /// @fn void has_signal_scroll::set_signal_scroll_blocked(bool)
    /// \signal_set_blocked
    ///
    /// @fn bool has_signal_scroll::get_signal_scroll_blocked() const
    /// \signal_get_blocked
    ///
    /// @fn void has_signal_scroll::disconnect_signal_scroll()
    /// \signal_disconnect
    ///
    /// @fn has_signal_scroll::has_signal_scroll
    /// \signal_ctor

    DECLARE_SIGNAL(ScrollEnd, scroll_end, SCROLL_END, "scroll-end", void);
    /// @class has_signal_scroll_end
    /// @brief signal emitted when the first frame after a user stops scrolling, emitted by mousetrap::ScrollEventController
    /// @tparam T instance type
    ///
    /// @fn void has_signal_scroll_end::emit_signal_scroll_end()
    /// \signal_emit_brief
    ///
    /// @var has_signal_scroll_end::signal_id
    /// \signal_id{https://docs.gtk.org/gtk4/signal.EventControllerScroll.scroll-begin.html}
    ///
    /// @var has_signal_scroll_end::signal_handler_with_data_f
    /// \signal_with_data_f{scroll_end}
    ///
    /// @var has_signal_scroll_end::signal_handler_without_data_f
    /// \signal_without_data_f{scroll_end}
    ///
    /// @fn void has_signal_scroll_end::connect_signal_scroll_end(const signal_handler_with_data_f<Data_t>& function, Data_t data)
    /// \signal_connect_data
    ///
    /// @fn void has_signal_scroll_end::connect_signal_scroll_end(const signal_handler_without_data_f&)
    /// \signal_connect_no_data
    ///
    /// @fn void has_signal_scroll_end::set_signal_scroll_end_blocked(bool)
    /// \signal_set_blocked
    ///
    /// @fn bool has_signal_scroll_end::get_signal_scroll_end_blocked() const
    /// \signal_get_blocked
    ///
    /// @fn void has_signal_scroll_end::disconnect_signal_scroll_end()
    /// \signal_disconnect
    ///
    /// @fn has_signal_scroll_end::has_signal_scroll_end
    /// \signal_ctor

    DECLARE_SIGNAL(FocusGained, focus_gained, FOCUS_GAINED, "enter", void);
    /// @class has_signal_focus_gained
    /// @brief signal emitted when a widget that is currently not focused becomes focused. Emitted by mousetrap::FocusEventController
    /// @tparam T instance type
    ///
    /// @fn void has_signal_focus_gained::emit_signal_focus_gained()
    /// \signal_emit_brief
    ///
    /// @var has_signal_focus_gained::signal_id
    /// \signal_id{https://docs.gtk.org/gtk4/signal.EventControllerFocus.enter.html}
    ///
    /// @var has_signal_focus_gained::signal_handler_with_data_f
    /// \signal_with_data_f{focus_gained}
    ///
    /// @var has_signal_focus_gained::signal_handler_without_data_f
    /// \signal_without_data_f{focus_gained}
    ///
    /// @fn void has_signal_focus_gained::connect_signal_focus_gained(const signal_handler_with_data_f<Data_t>& function, Data_t data)
    /// \signal_connect_data
    ///
    /// @fn void has_signal_focus_gained::connect_signal_focus_gained(const signal_handler_without_data_f&)
    /// \signal_connect_no_data
    ///
    /// @fn void has_signal_focus_gained::set_signal_focus_gained_blocked(bool)
    /// \signal_set_blocked
    ///
    /// @fn bool has_signal_focus_gained::get_signal_focus_gained_blocked() const
    /// \signal_get_blocked
    ///
    /// @fn void has_signal_focus_gained::disconnect_signal_focus_gained()
    /// \signal_disconnect
    ///
    /// @fn has_signal_focus_gained::has_signal_focus_gained
    /// \signal_ctor

    DECLARE_SIGNAL(FocusLost, focus_lost, FOCUS_LOST, "leave", void);
    /// @class has_signal_focus_lost
    /// @brief signal emitted when a widget that is currently focused looses focus to another widget. Emitted by mousetrap::FocusEventController
    /// @tparam T instance type
    ///
    /// @fn void has_signal_focus_lost::emit_signal_focus_lost()
    /// \signal_emit_brief
    ///
    /// @var has_signal_focus_lost::signal_id
    /// \signal_id{https://docs.gtk.org/gtk4/signal.EventControllerFocus.leave.html}
    ///
    /// @var has_signal_focus_lost::signal_handler_with_data_f
    /// \signal_with_data_f{focus_lost}
    ///
    /// @var has_signal_focus_lost::signal_handler_without_data_f
    /// \signal_without_data_f{focus_lost}
    ///
    /// @fn void has_signal_focus_lost::connect_signal_focus_lost(const signal_handler_with_data_f<Data_t>& function, Data_t data)
    /// \signal_connect_data
    ///
    /// @fn void has_signal_focus_lost::connect_signal_focus_lost(const signal_handler_without_data_f&)
    /// \signal_connect_no_data
    ///
    /// @fn void has_signal_focus_lost::set_signal_focus_lost_blocked(bool)
    /// \signal_set_blocked
    ///
    /// @fn bool has_signal_focus_lost::get_signal_focus_lost_blocked() const
    /// \signal_get_blocked
    ///
    /// @fn void has_signal_focus_lost::disconnect_signal_focus_lost()
    /// \signal_disconnect
    ///
    /// @fn has_signal_focus_lost::has_signal_focus_lost
    /// \signal_ctor

    DECLARE_SIGNAL_MANUAL(drag_begin, "drag-begin", void,
        SPLAT(double start_x, double start_y),
        SPLAT(start_x, start_y)
    );
    /// @class has_signal_drag_begin
    /// @brief signal emitted when the user initiates a drag over a widget that has a mousetrap::DragEventController connected
    /// @tparam T instance type
    ///
    /// @fn void has_signal_drag_begin::emit_signal_drag_begin(double start_x, double start_y)
    /// \signal_emit_brief
    /// @param start_x x-position of the cursor, widget-relative coordinates, in pixels
    /// @param start_y y-position of the cursor, widget-relative coordiantes, in pixels
    ///
    /// @var has_signal_drag_begin::signal_id
    /// \signal_id{https://docs.gtk.org/gtk4/signal.GestureDrag.drag-begin.html}
    ///
    /// @var has_signal_drag_begin::signal_handler_with_data_f
    /// \signal_with_data_f{drag_begin}
    /// \signal_see_emit_for_parameters{drag_begin}
    ///
    /// @var has_signal_drag_begin::signal_handler_without_data_f
    /// \signal_without_data_f{drag_begin}
    /// \signal_see_emit_for_parameters{drag_begin}
    ///
    /// @fn void has_signal_drag_begin::connect_signal_drag_begin(const signal_handler_with_data_f<Data_t>& function, Data_t data)
    /// \signal_connect_data
    /// \signal_see_emit_for_parameters{text_inserted}
    ///
    /// @fn void has_signal_drag_begin::connect_signal_drag_begin(const signal_handler_without_data_f&)
    /// \signal_connect_no_data
    /// \signal_see_emit_for_parameters{text_inserted}
    ///
    /// @fn void has_signal_drag_begin::set_signal_drag_begin_blocked(bool)
    /// \signal_set_blocked
    ///
    /// @fn bool has_signal_drag_begin::get_signal_drag_begin_blocked() const
    /// \signal_get_blocked
    ///
    /// @fn void has_signal_drag_begin::disconnect_signal_drag_begin()
    /// \signal_disconnect
    ///
    /// @fn has_signal_drag_begin::has_signal_drag_begin
    /// \signal_ctor

    DECLARE_SIGNAL_MANUAL(drag, "drag-update", void,
        SPLAT(double offset_x, double offset_y),
        SPLAT(offset_x, offset_y)
    );
    /// @class has_signal_drag
    /// @brief signal emitted when once per frame while a drag is active
    /// @tparam T instance type
    ///
    /// @fn void has_signal_drag::emit_signal_drag(double offset_x, double offset_y)
    /// \signal_emit_brief
    /// @param offset_x horizontal offset from drag start point, widget-relative coordinates, in pixels
    /// @param offset_y vertical offset from drag start point, widget-relative coordinates, in pixels
    ///
    /// @var has_signal_drag::signal_id
    /// \signal_id{https://docs.gtk.org/gtk4/signal.GestureDrag.drag-update.html}
    ///
    /// @var has_signal_drag::signal_handler_with_data_f
    /// \signal_with_data_f{drag}
    /// \signal_see_emit_for_parameters{drag}
    ///
    /// @var has_signal_drag::signal_handler_without_data_f
    /// \signal_without_data_f{drag}
    /// \signal_see_emit_for_parameters{drag}
    ///
    /// @fn void has_signal_drag::connect_signal_drag(const signal_handler_with_data_f<Data_t>& function, Data_t data)
    /// \signal_connect_data
    /// \signal_see_emit_for_parameters{text_inserted}
    ///
    /// @fn void has_signal_drag::connect_signal_drag(const signal_handler_without_data_f&)
    /// \signal_connect_no_data
    /// \signal_see_emit_for_parameters{text_inserted}
    ///
    /// @fn void has_signal_drag::set_signal_drag_blocked(bool)
    /// \signal_set_blocked
    ///
    /// @fn bool has_signal_drag::get_signal_drag_blocked() const
    /// \signal_get_blocked
    ///
    /// @fn void has_signal_drag::disconnect_signal_drag()
    /// \signal_disconnect
    ///
    /// @fn has_signal_drag::has_signal_drag
    /// \signal_ctor

    DECLARE_SIGNAL_MANUAL(drag_end, "drag-end", void,
        SPLAT(double offset_x, double offest_y),
        SPLAT(offset_x, offest_y)
    );
    /// @class has_signal_drag_end
    /// @brief signal emitted when when drag ends, usually when the user seizes to press the mouse button
    /// @tparam T instance type
    ///
    /// @fn void has_signal_drag_end::emit_signal_drag_end(double offset_x, double offest_y)
    /// \signal_emit_brief
    /// @param offset_x horizontal offset from drag start point, widget-relative coordinates, in pixels
    /// @param offset_y vertical offset from drag start point, widget-relative coordinates, in pixels
    ///
    /// @var has_signal_drag_end::signal_id
    /// \signal_id{https://docs.gtk.org/gtk4/signal.GestureDrag.drag-end.html}
    ///
    /// @var has_signal_drag_end::signal_handler_with_data_f
    /// \signal_with_data_f{drag_end}
    /// \signal_see_emit_for_parameters{drag_end}
    ///
    /// @var has_signal_drag_end::signal_handler_without_data_f
    /// \signal_without_data_f{drag_end}
    /// \signal_see_emit_for_parameters{drag_end}
    ///
    /// @fn void has_signal_drag_end::connect_signal_drag_end(const signal_handler_with_data_f<Data_t>& function, Data_t data)
    /// \signal_connect_data
    /// \signal_see_emit_for_parameters{text_inserted}
    ///
    /// @fn void has_signal_drag_end::connect_signal_drag_end(const signal_handler_without_data_f&)
    /// \signal_connect_no_data
    /// \signal_see_emit_for_parameters{text_inserted}
    ///
    /// @fn void has_signal_drag_end::set_signal_drag_end_blocked(bool)
    /// \signal_set_blocked
    ///
    /// @fn bool has_signal_drag_end::get_signal_drag_end_blocked() const
    /// \signal_get_blocked
    ///
    /// @fn void has_signal_drag_end::disconnect_signal_drag_end()
    /// \signal_disconnect
    ///
    /// @fn has_signal_drag_end::has_signal_drag_end
    /// \signal_ctor

    DECLARE_SIGNAL_MANUAL(scale_changed, "scale-changed", void,
        double scale,
        scale
    );
    /// @class has_signal_scale_changed
    /// @brief signal emitted when the distance between two fingers of the "Pinch-Zoom" gesture changes
    /// @tparam T instance type
    ///
    /// @fn void has_signal_scale_changed::emit_signal_scale_changed(double scale)
    /// \signal_emit_brief
    /// @param scale difference of current scale to scale at the start of the gesture
    ///
    /// @var has_signal_scale_changed::signal_id
    /// \signal_id{https://docs.gtk.org/gtk4/signal.GestureZoom.scale-changed.html}
    ///
    /// @var has_signal_scale_changed::signal_handler_with_data_f
    /// \signal_with_data_f{scale_changed}
    /// \signal_see_emit_for_parameters{scale_changed}
    ///
    /// @var has_signal_scale_changed::signal_handler_without_data_f
    /// \signal_without_data_f{scale_changed}
    /// \signal_see_emit_for_parameters{scale_changed}
    ///
    /// @fn void has_signal_scale_changed::connect_signal_scale_changed(const signal_handler_with_data_f<Data_t>& function, Data_t data)
    /// \signal_connect_data
    /// \signal_see_emit_for_parameters{text_inserted}
    ///
    /// @fn void has_signal_scale_changed::connect_signal_scale_changed(const signal_handler_without_data_f&)
    /// \signal_connect_no_data
    /// \signal_see_emit_for_parameters{text_inserted}
    ///
    /// @fn void has_signal_scale_changed::set_signal_scale_changed_blocked(bool)
    /// \signal_set_blocked
    ///
    /// @fn bool has_signal_scale_changed::get_signal_scale_changed_blocked() const
    /// \signal_get_blocked
    ///
    /// @fn void has_signal_scale_changed::disconnect_signal_scale_changed()
    /// \signal_disconnect
    ///
    /// @fn has_signal_scale_changed::has_signal_scale_changed
    /// \signal_ctor

    DECLARE_SIGNAL_MANUAL(rotation_changed, "angle-changed", void,
        SPLAT(double angle_absolute_radians, double angle_delta_radians),
        SPLAT(angle_absolute_radians, angle_delta_radians)
    );
    /// @class has_signal_rotation_changed
    /// @brief signal emitted when angel of two-finger-rotation-gestured changes
    /// @tparam T instance type
    ///
    /// @fn void has_signal_rotation_changed::emit_signal_rotation_changed(double angle_absolute_radians, double angle_delta_radians)
    /// \signal_emit_brief
    /// @param angle_absolute_radians current angle relative to widget coordinate origin, in radians
    /// @param angle_delta_radians difference between the angle this frame and that of the last frame
    ///
    /// @var has_signal_rotation_changed::signal_id
    /// \signal_id{https://docs.gtk.org/gtk4/signal.GestureRotate.angle-changed.html}
    ///
    /// @var has_signal_rotation_changed::signal_handler_with_data_f
    /// \signal_with_data_f{rotation_changed}
    /// \signal_see_emit_for_parameters{rotation_changed}
    ///
    /// @var has_signal_rotation_changed::signal_handler_without_data_f
    /// \signal_without_data_f{rotation_changed}
    /// \signal_see_emit_for_parameters{rotation_changed}
    ///
    /// @fn void has_signal_rotation_changed::connect_signal_rotation_changed(const signal_handler_with_data_f<Data_t>& function, Data_t data)
    /// \signal_connect_data
    /// \signal_see_emit_for_parameters{text_inserted}
    ///
    /// @fn void has_signal_rotation_changed::connect_signal_rotation_changed(const signal_handler_without_data_f&)
    /// \signal_connect_no_data
    /// \signal_see_emit_for_parameters{text_inserted}
    ///
    /// @fn void has_signal_rotation_changed::set_signal_rotation_changed_blocked(bool)
    /// \signal_set_blocked
    ///
    /// @fn bool has_signal_rotation_changed::get_signal_rotation_changed_blocked() const
    /// \signal_get_blocked
    ///
    /// @fn void has_signal_rotation_changed::disconnect_signal_rotation_changed()
    /// \signal_disconnect
    ///
    /// @fn has_signal_rotation_changed::has_signal_rotation_changed
    /// \signal_ctor

    DECLARE_SIGNAL(PropertiesChanged, properties_changed, PROPERTIES_CHANGED, "changed", void);
    /// @class has_signal_properties_changed
    /// @brief signal emitted when properties other than the "value" property of a mousetrap::Adjustment change
    /// @tparam T instance type
    /// @see mousetrap::has_signal_value_changed
    ///
    /// @fn void has_signal_properties_changed::emit_signal_properties_changed()
    /// \signal_emit_brief
    ///
    /// @var has_signal_properties_changed::signal_id
    /// \signal_id{https://docs.gtk.org/gtk4/signal.Adjustment.changed.html}
    ///
    /// @var has_signal_properties_changed::signal_handler_with_data_f
    /// \signal_with_data_f{properties_changed}
    ///
    /// @var has_signal_properties_changed::signal_handler_without_data_f
    /// \signal_without_data_f{properties_changed}
    ///
    /// @fn void has_signal_properties_changed::connect_signal_properties_changed(const signal_handler_with_data_f<Data_t>& function, Data_t data)
    /// \signal_connect_data
    ///
    /// @fn void has_signal_properties_changed::connect_signal_properties_changed(const signal_handler_without_data_f&)
    /// \signal_connect_no_data
    ///
    /// @fn void has_signal_properties_changed::set_signal_properties_changed_blocked(bool)
    /// \signal_set_blocked
    ///
    /// @fn bool has_signal_properties_changed::get_signal_properties_changed_blocked() const
    /// \signal_get_blocked
    ///
    /// @fn void has_signal_properties_changed::disconnect_signal_properties_changed()
    /// \signal_disconnect
    ///
    /// @fn has_signal_properties_changed::has_signal_properties_changed
    /// \signal_ctor

    DECLARE_SIGNAL(ValueChanged, value_changed, VALUE_CHANGED, "value-changed", void);
    /// @class has_signal_value_changed
    /// @brief signal emitted when the value of a widget representing a range of values changes
    /// @tparam T instance type
    ///
    /// @fn void has_signal_value_changed::emit_signal_value_changed()
    /// \signal_emit_brief
    ///
    /// @var has_signal_value_changed::signal_id
    /// \signal_id{https://docs.gtk.org/gtk4/signal.Adjustment.value-changed.html}
    ///
    /// @var has_signal_value_changed::signal_handler_with_data_f
    /// \signal_with_data_f{value_changed}
    ///
    /// @var has_signal_value_changed::signal_handler_without_data_f
    /// \signal_without_data_f{value_changed}
    ///
    /// @fn void has_signal_value_changed::connect_signal_value_changed(const signal_handler_with_data_f<Data_t>& function, Data_t data)
    /// \signal_connect_data
    ///
    /// @fn void has_signal_value_changed::connect_signal_value_changed(const signal_handler_without_data_f&)
    /// \signal_connect_no_data
    ///
    /// @fn void has_signal_value_changed::set_signal_value_changed_blocked(bool)
    /// \signal_set_blocked
    ///
    /// @fn bool has_signal_value_changed::get_signal_value_changed_blocked() const
    /// \signal_get_blocked
    ///
    /// @fn void has_signal_value_changed::disconnect_signal_value_changed()
    /// \signal_disconnect
    ///
    /// @fn has_signal_value_changed::has_signal_value_changed
    /// \signal_ctor

    DECLARE_SIGNAL_MANUAL(render, "render", bool, GdkGLContext* context, context);
    /// @class has_signal_render
    /// @brief signal emitted when a mousetrap::RenderArea enters its draw cycle. If the user does not connect to this signal, the area will automatically render all objects registered via mousetrap::RenderArea::add_render_task
    /// @tparam T instance type
    ///
    /// @fn void has_signal_render::emit_signal_render()
    /// \signal_emit_brief
    ///
    /// @var has_signal_render::signal_id
    /// \signal_id{https://docs.gtk.org/gtk4/signal.GLArea.render.html}
    ///
    /// @var has_signal_render::signal_handler_with_data_f
    /// \signal_with_data_f{render}
    ///
    /// @var has_signal_render::signal_handler_without_data_f
    /// \signal_without_data_f{render}
    ///
    /// @fn void has_signal_render::connect_signal_render(const signal_handler_with_data_f<Data_t>& function, Data_t data)
    /// \signal_connect_data
    ///
    /// @fn void has_signal_render::connect_signal_render(const signal_handler_without_data_f&)
    /// \signal_connect_no_data
    ///
    /// @fn void has_signal_render::set_signal_render_blocked(bool)
    /// \signal_set_blocked
    ///
    /// @fn bool has_signal_render::get_signal_render_blocked() const
    /// \signal_get_blocked
    ///
    /// @fn void has_signal_render::disconnect_signal_render()
    /// \signal_disconnect
    ///
    /// @fn has_signal_render::has_signal_render
    /// \signal_ctor

    DECLARE_SIGNAL_MANUAL(resize, "resize", void,
        SPLAT(int32_t width, int32_t height),
        SPLAT(width, height)
    );
    /// @class has_signal_resize
    /// @brief signal emitted when a mousetrap::RenderArea changes size on screen
    /// @tparam T instance type
    ///
    /// @fn void has_signal_resize::emit_signal_resize(int32_t width, int32_t height)
    /// \signal_emit_brief
    /// @param width absolute width, in pixels
    /// @param height absolute height, in pixels
    ///
    /// @var has_signal_resize::signal_id
    /// \signal_id{https://docs.gtk.org/gtk4/signal.GLArea.resize.html}
    ///
    /// @var has_signal_resize::signal_handler_with_data_f
    /// \signal_with_data_f{resize}
    /// \signal_see_emit_for_parameters{resize}
    ///
    /// @var has_signal_resize::signal_handler_without_data_f
    /// \signal_without_data_f{resize}
    /// \signal_see_emit_for_parameters{resize}
    ///
    /// @fn void has_signal_resize::connect_signal_resize(const signal_handler_with_data_f<Data_t>& function, Data_t data)
    /// \signal_connect_data
    /// \signal_see_emit_for_parameters{text_inserted}
    ///
    /// @fn void has_signal_resize::connect_signal_resize(const signal_handler_without_data_f&)
    /// \signal_connect_no_data
    /// \signal_see_emit_for_parameters{text_inserted}
    ///
    /// @fn void has_signal_resize::set_signal_resize_blocked(bool)
    /// \signal_set_blocked
    ///
    /// @fn bool has_signal_resize::get_signal_resize_blocked() const
    /// \signal_get_blocked
    ///
    /// @fn void has_signal_resize::disconnect_signal_resize()
    /// \signal_disconnect
    ///
    /// @fn has_signal_resize::has_signal_resize
    /// \signal_ctor

    DECLARE_SIGNAL_MANUAL(page_added, "page-added", void,
        SPLAT(GtkWidget* _, uint32_t page_index),
        SPLAT(_, page_index)
    );
    /// @class has_signal_page_added
    /// @brief signal emitted when a page is added to a mousetrap::Notebook
    /// @tparam T instance type
    ///
    /// @fn void has_signal_page_added::emit_signal_page_added(GtkWidget* _, uint32_t page_index)
    /// \signal_emit_brief
    /// @param _ pointer to internal widget, can be ignored
    /// @param page_index index of the new page
    ///
    /// @var has_signal_page_added::signal_id
    /// \signal_id{https://docs.gtk.org/gtk4/signal.Notebook.page-added.html}
    ///
    /// @var has_signal_page_added::signal_handler_with_data_f
    /// \signal_with_data_f{page_added}
    /// \signal_see_emit_for_parameters{page_added}
    ///
    /// @var has_signal_page_added::signal_handler_without_data_f
    /// \signal_without_data_f{page_added}
    /// \signal_see_emit_for_parameters{page_added}
    ///
    /// @fn void has_signal_page_added::connect_signal_page_added(const signal_handler_with_data_f<Data_t>& function, Data_t data)
    /// \signal_connect_data
    /// \signal_see_emit_for_parameters{text_inserted}
    ///
    /// @fn void has_signal_page_added::connect_signal_page_added(const signal_handler_without_data_f&)
    /// \signal_connect_no_data
    /// \signal_see_emit_for_parameters{text_inserted}
    ///
    /// @fn void has_signal_page_added::set_signal_page_added_blocked(bool)
    /// \signal_set_blocked
    ///
    /// @fn bool has_signal_page_added::get_signal_page_added_blocked() const
    /// \signal_get_blocked
    ///
    /// @fn void has_signal_page_added::disconnect_signal_page_added()
    /// \signal_disconnect
    ///
    /// @fn has_signal_page_added::has_signal_page_added
    /// \signal_ctor

    DECLARE_SIGNAL_MANUAL(page_removed, "page-removed", void,
        SPLAT(GtkWidget* _, uint32_t page_index),
        SPLAT(_, page_index)
    );
    /// @class has_signal_page_removed
    /// @brief signal emitted when a page is removed from a mousetrap::Notebook
    /// @tparam T instance type
    ///
    /// @fn void has_signal_page_removed::emit_signal_page_removed(GtkWidget* _, uint32_t page_index)
    /// \signal_emit_brief
    /// @param _ pointer to internal widget, can be ignored
    /// @param page_index index of the new page
    ///
    /// @var has_signal_page_removed::signal_id
    /// \signal_id{https://docs.gtk.org/gtk4/signal.Notebook.page-removed.html}
    ///
    /// @var has_signal_page_removed::signal_handler_with_data_f
    /// \signal_with_data_f{page_removed}
    /// \signal_see_emit_for_parameters{page_removed}
    ///
    /// @var has_signal_page_removed::signal_handler_without_data_f
    /// \signal_without_data_f{page_removed}
    /// \signal_see_emit_for_parameters{page_removed}
    ///
    /// @fn void has_signal_page_removed::connect_signal_page_removed(const signal_handler_with_data_f<Data_t>& function, Data_t data)
    /// \signal_connect_data
    /// \signal_see_emit_for_parameters{text_inserted}
    ///
    /// @fn void has_signal_page_removed::connect_signal_page_removed(const signal_handler_without_data_f&)
    /// \signal_connect_no_data
    /// \signal_see_emit_for_parameters{text_inserted}
    ///
    /// @fn void has_signal_page_removed::set_signal_page_removed_blocked(bool)
    /// \signal_set_blocked
    ///
    /// @fn bool has_signal_page_removed::get_signal_page_removed_blocked() const
    /// \signal_get_blocked
    ///
    /// @fn void has_signal_page_removed::disconnect_signal_page_removed()
    /// \signal_disconnect
    ///
    /// @fn has_signal_page_removed::has_signal_page_removed
    /// \signal_ctor

    DECLARE_SIGNAL_MANUAL(page_reordered, "page-reordered", void,
        SPLAT(GtkWidget* widget, uint32_t page_index),
        SPLAT(widget, page_index)
    );
    /// @class has_signal_page_reordered
    /// @brief signal emitted when the position of a page of a mousetrap::Notebook changes
    /// @tparam T instance type
    ///
    /// @fn void has_signal_page_reordered::emit_signal_page_reordered(GtkWidget* _, uint32_t page_index)
    /// \signal_emit_brief
    /// @param _ pointer to internal widget, can be ignored
    /// @param page_index index of the new page
    ///
    /// @var has_signal_page_reordered::signal_id
    /// \signal_id{https://docs.gtk.org/gtk4/signal.Notebook.page-reordered.html}
    ///
    /// @var has_signal_page_reordered::signal_handler_with_data_f
    /// \signal_with_data_f{page_reordered}
    /// \signal_see_emit_for_parameters{page_reordered}
    ///
    /// @var has_signal_page_reordered::signal_handler_without_data_f
    /// \signal_without_data_f{page_reordered}
    /// \signal_see_emit_for_parameters{page_reordered}
    ///
    /// @fn void has_signal_page_reordered::connect_signal_page_reordered(const signal_handler_with_data_f<Data_t>& function, Data_t data)
    /// \signal_connect_data
    /// \signal_see_emit_for_parameters{text_inserted}
    ///
    /// @fn void has_signal_page_reordered::connect_signal_page_reordered(const signal_handler_without_data_f&)
    /// \signal_connect_no_data
    /// \signal_see_emit_for_parameters{text_inserted}
    ///
    /// @fn void has_signal_page_reordered::set_signal_page_reordered_blocked(bool)
    /// \signal_set_blocked
    ///
    /// @fn bool has_signal_page_reordered::get_signal_page_reordered_blocked() const
    /// \signal_get_blocked
    ///
    /// @fn void has_signal_page_reordered::disconnect_signal_page_reordered()
    /// \signal_disconnect
    ///
    /// @fn has_signal_page_reordered::has_signal_page_reordered
    /// \signal_ctor

    DECLARE_SIGNAL_MANUAL(page_selection_changed, "switch-page", void,
        SPLAT(GtkWidget* _, uint32_t page_index),
        SPLAT(_, page_index)
    );
    /// @class has_signal_page_selection_changed
    /// @brief signal emitted when page selection of a mousetrap::Notebook changes
    /// @tparam T instance type
    ///
    /// @fn void has_signal_page_selection_changed::emit_signal_page_selection_changed(GtkWidget* _, uint32_t page_index)
    /// \signal_emit_brief
    /// @param _ pointer to internal widget, can be ignored
    /// @param page_index index of the newly selected page
    ///
    /// @var has_signal_page_selection_changed::signal_id
    /// \signal_id{https://docs.gtk.org/gtk4/signal.Notebook.switch-page.html}
    ///
    /// @var has_signal_page_selection_changed::signal_handler_with_data_f
    /// \signal_with_data_f{page_selection_changed}
    /// \signal_see_emit_for_parameters{page_selection_changed}
    ///
    /// @var has_signal_page_selection_changed::signal_handler_without_data_f
    /// \signal_without_data_f{page_selection_changed}
    /// \signal_see_emit_for_parameters{page_selection_changed}
    ///
    /// @fn void has_signal_page_selection_changed::connect_signal_page_selection_changed(const signal_handler_with_data_f<Data_t>& function, Data_t data)
    /// \signal_connect_data
    /// \signal_see_emit_for_parameters{text_inserted}
    ///
    /// @fn void has_signal_page_selection_changed::connect_signal_page_selection_changed(const signal_handler_without_data_f&)
    /// \signal_connect_no_data
    /// \signal_see_emit_for_parameters{text_inserted}
    ///
    /// @fn void has_signal_page_selection_changed::set_signal_page_selection_changed_blocked(bool)
    /// \signal_set_blocked
    ///
    /// @fn bool has_signal_page_selection_changed::get_signal_page_selection_changed_blocked() const
    /// \signal_get_blocked
    ///
    /// @fn void has_signal_page_selection_changed::disconnect_signal_page_selection_changed()
    /// \signal_disconnect
    ///
    /// @fn has_signal_page_selection_changed::has_signal_page_selection_changed
    /// \signal_ctor

    DECLARE_SIGNAL(Wrapped, wrapped, WRAPPED, "wrapped", void);
    /// @class has_signal_wrapped
    /// @brief signal emitted when a spin buttons values "wraps", meaning it overflows from the lowest possible value to the highest, or vice-versa. This signal is not emitted if mousetrap::SpinButton::set_should_wrap was set to false
    /// @tparam T instance type
    ///
    /// @fn void has_signal_wrapped::emit_signal_wrapped()
    /// \signal_emit_brief
    ///
    /// @var has_signal_wrapped::signal_id
    /// \signal_id{https://docs.gtk.org/gtk4/signal.SpinButton.wrapped.html}
    ///
    /// @var has_signal_wrapped::signal_handler_with_data_f
    /// \signal_with_data_f{wrapped}
    ///
    /// @var has_signal_wrapped::signal_handler_without_data_f
    /// \signal_without_data_f{wrapped}
    ///
    /// @fn void has_signal_wrapped::connect_signal_wrapped(const signal_handler_with_data_f<Data_t>& function, Data_t data)
    /// \signal_connect_data
    ///
    /// @fn void has_signal_wrapped::connect_signal_wrapped(const signal_handler_without_data_f&)
    /// \signal_connect_no_data
    ///
    /// @fn void has_signal_wrapped::set_signal_wrapped_blocked(bool)
    /// \signal_set_blocked
    ///
    /// @fn bool has_signal_wrapped::get_signal_wrapped_blocked() const
    /// \signal_get_blocked
    ///
    /// @fn void has_signal_wrapped::disconnect_signal_wrapped()
    /// \signal_disconnect
    ///
    /// @fn has_signal_wrapped::has_signal_wrapped
    /// \signal_ctor

    DECLARE_SIGNAL_MANUAL(response, "response", void, int response, response);
    /// @class has_signal_response
    /// @brief signal emitted when the users choosed an action of a dialog
    /// @tparam T instance type
    ///
    /// @fn void has_signal_response::emit_signal_response(int response)
    /// \signal_emit_brief
    /// @param response response id as int
    ///
    /// @var has_signal_response::signal_id
    /// \signal_id{https://docs.gtk.org/gtk4/signal.NativeDialog.response.html}
    ///
    /// @var has_signal_response::signal_handler_with_data_f
    /// \signal_with_data_f{response}
    /// \signal_see_emit_for_parameters{response}
    ///
    /// @var has_signal_response::signal_handler_without_data_f
    /// \signal_without_data_f{response}
    /// \signal_see_emit_for_parameters{response}
    ///
    /// @fn void has_signal_response::connect_signal_response(const signal_handler_with_data_f<Data_t>& function, Data_t data)
    /// \signal_connect_data
    /// \signal_see_emit_for_parameters{text_inserted}
    ///
    /// @fn void has_signal_response::connect_signal_response(const signal_handler_without_data_f&)
    /// \signal_connect_no_data
    /// \signal_see_emit_for_parameters{text_inserted}
    ///
    /// @fn void has_signal_response::set_signal_response_blocked(bool)
    /// \signal_set_blocked
    ///
    /// @fn bool has_signal_response::get_signal_response_blocked() const
    /// \signal_get_blocked
    ///
    /// @fn void has_signal_response::disconnect_signal_response()
    /// \signal_disconnect
    ///
    /// @fn has_signal_response::has_signal_response
    /// \signal_ctor
}
