//
// Copyright (c) Clemens Cords (mail@clemens-cords.com), created 3/17/23
//

#pragma once

#include <functional>

namespace mousetrap
{
    #define SPLAT(...) __VA_ARGS__

    #define HAS_SIGNAL(T, signal_name) \
        public has_##signal_name##_signal<T>

    #define CTOR_SIGNAL(T, signal_name) \
        has_##signal_name##_signal<T>(this)

    /// @brief declare a signal with the signature (T* instance, auto data) -> return_t
    /// @param signal_name name of the signal, has to be a valid C++ variable name
    /// @param g_signal_id Gio ID of the signal, string
    /// @param return_type return type of the signal wrapper function
    #define DECLARE_SIGNAL(signal_name, g_signal_id, return_type)                                     \
        template<typename T>                                                                          \
        class has_##signal_name##_signal                                                              \
        {                                                                                             \
            private:                                                                                  \
                T* _instance = nullptr;                                                               \
                std::function<return_type(T*)> _function;                                             \
                bool _blocked = false;                                                                \
                                                                                                      \
                static return_type wrapper(void*, has_##signal_name##_signal<T>* instance)            \
                {                                                                                     \
                    return instance->emit_signal_activate();                                          \
                }                                                                                     \
                                                                                                      \
            protected:                                                                                \
                explicit has_##signal_name##_signal(T* instance)                                      \
                    : _instance(instance)                                                             \
                {}                                                                                    \
                                                                                                      \
            public:                                                                                   \
                static inline constexpr const char* signal_id = g_signal_id;                          \
                                                                                                      \
                template<typename Function_t, typename Data_t>                                        \
                void connect_signal_##signal_name(Function_t function, Data_t data)                   \
                {                                                                                     \
                    _function = [f = function, d = data](T* instance) -> return_type                  \
                    {                                                                                 \
                        return f(instance, d);                                                                  \
                    };                                                                                \
                                                                                                      \
                    static_cast<SignalEmitter*>(_instance)->connect_signal(signal_id, wrapper, this); \
                }                                                                                     \
                                                                                                      \
                template<typename Function_t>                                                         \
                void connect_signal_##signal_name(Function_t function)                                \
                {                                                                                     \
                    _function = [f = function](T* instance) -> return_type                            \
                    {                                                                                 \
                        return f(instance);                                                           \
                    };                                                                                \
                                                                                                      \
                    static_cast<SignalEmitter*>(_instance)->connect_signal(signal_id, wrapper, this); \
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
                return_type emit_signal_activate()                                                    \
                {                                                                                     \
                    if (not _blocked)                                                                 \
                        return _function(_instance);                                                  \
                    else                                                                              \
                        return return_type();                                                         \
                }                                                                                     \
        }

    /// @brief declare a signal with the signature (T* instance, arg_list..., auto data) -> return_t
    /// @param signal_name name of the signal, has to be a valid C++ variable name
    /// @param g_signal_id Gio ID of the signal, string
    /// @param return_type return type of the signal wrapper function
    /// @param arg_list list of argument types an names, for example `float x, float y`
    /// @param arg_name_list list of arguments **without** the type, for example `x, y`
    #define DECLARE_SIGNAL_MANUAL(signal_name, g_signal_id, return_t, arg_list, arg_name_list)        \
        template<typename T>                                                                          \
        class has_##signal_name##_signal                                                              \
        {                                                                                             \
            private:                                                                                  \
                T* _instance = nullptr;                                                               \
                std::function<return_t(T* instance, arg_list)> _function;                             \
                bool _blocked = false;                                                                \
                                                                                                      \
                static return_t wrapper(void*, arg_list, has_##signal_name##_signal<T>* self)         \
                {                                                                                     \
                    self->emit_signal_activate(arg_name_list);                                        \
                }                                                                                     \
                                                                                                      \
            protected:                                                                                \
                explicit has_##signal_name##_signal(T* instance)                                      \
                    : _instance(instance)                                                             \
                {}                                                                                    \
                                                                                                      \
            public:                                                                                   \
                static inline constexpr const char* signal_id = g_signal_id;                          \
                                                                                                      \
                template<typename Function_t, typename Data_t>                                        \
                void connect_signal_##signal_name(Function_t function, Data_t data)                   \
                {                                                                                     \
                    _function = [f = function, d = data](T* instance, arg_list)                       \
                    {                                                                                 \
                        return f(instance, arg_name_list, d);                                            \
                    };                                                                                \
                    static_cast<SignalEmitter*>(_instance)->connect_signal(signal_id, wrapper, this); \
                }                                                                                     \
                                                                                                      \
                template<typename Function_t>                                                         \
                void connect_signal_##signal_name(Function_t function)                                \
                {                                                                                     \
                    _function = [f = function](T* instance, arg_list)                                 \
                    {                                                                                 \
                        return f(instance, arg_name_list);                                            \
                    };                                                                                \
                    static_cast<SignalEmitter*>(_instance)->connect_signal(signal_id, wrapper, this); \
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
                return_t emit_signal_activate(arg_list)                                               \
                {                                                                                     \
                    if (not _blocked and _function != nullptr)                                        \
                        return _function(_instance, arg_name_list);                                   \
                    else                                                                              \
                        return return_t();                                                            \
                }                                                                                     \
        }
    
    /// @see https://docs.gtk.org/gio/signal.Application.activate.html
    DECLARE_SIGNAL(activate, "activate", void);

    /// @see https://docs.gtk.org/gio/signal.Application.startup.html
    DECLARE_SIGNAL(startup, "startup", void);

    /// @see https://docs.gtk.org/gio/signal.Application.startup.html
    DECLARE_SIGNAL(shutdown, "shutdown", void);

    /// @see https://docs.gtk.org/gdk4/signal.FrameClock.update.html
    DECLARE_SIGNAL(update, "update", void);

    /// @see https://docs.gtk.org/gdk4/signal.FrameClock.paint.html
    DECLARE_SIGNAL(paint, "paint", void);

    /// @see https://docs.gtk.org/gtk4/signal.Widget.realize.html
    DECLARE_SIGNAL(realize, "realize", void);

    /// @see https://docs.gtk.org/gtk4/signal.Widget.unrealize.html
    DECLARE_SIGNAL(unrealize, "unrealize", void);

    /// @see https://docs.gtk.org/gtk4/signal.Widget.destroy.html
    DECLARE_SIGNAL(destroy, "destroy", void);

    /// @see https://docs.gtk.org/gtk4/signal.Widget.hide.html
    DECLARE_SIGNAL(hide, "hide", void);

    /// @see https://docs.gtk.org/gtk4/signal.Widget.show.html
    DECLARE_SIGNAL(show, "show", void);

    /// @see https://docs.gtk.org/gtk4/signal.Widget.map.html
    DECLARE_SIGNAL(map, "map", void);

    /// @see https://docs.gtk.org/gtk4/signal.Widget.unmap.html
    DECLARE_SIGNAL(unmap, "unmap", void);

    /// @see https://docs.gtk.org/gtk4/signal.Window.close-request.html
    DECLARE_SIGNAL(close_request, "close-request", bool);

    /// @see https://docs.gtk.org/gtk4/signal.Window.activate-default.html
    DECLARE_SIGNAL(activate_default_widget, "activate-default", void);

    /// @see https://docs.gtk.org/gtk4/signal.Window.activate-focus.html
    DECLARE_SIGNAL(activate_focused_widget, "activate-focus", void);

    /// @see https://docs.gtk.org/gtk4/signal.Button.activate.html
    DECLARE_SIGNAL(clicked, "clicked", void);

    /// @see https://docs.gtk.org/gtk4/signal.ToggleButton.toggled.html
    DECLARE_SIGNAL(toggled, "toggled", void);

    /// @see https://docs.gtk.org/gtk4/signal.Editable.changed.html
    DECLARE_SIGNAL(text_changed, "changed", void);

    /// @see https://docs.gtk.org/gtk4/signal.Editable.insert-text.html
    DECLARE_SIGNAL_MANUAL(text_inserted, "insert-text", void,
          SPLAT(int32_t start_pos, int32_t end_pos),
          SPLAT(start_pos, end_pos)
    );

    /// @see https://docs.gtk.org/gtk4/signal.Editable.delete-text.html
    DECLARE_SIGNAL_MANUAL(text_deleted, "delete-text", void,
          SPLAT(int32_t start_pos, int32_t end_pos),
          SPLAT(start_pos, end_pos)
    );

    /// @see https://docs.gtk.org/gtk4/signal.TextBuffer.undo.html
    DECLARE_SIGNAL(undo, "undo", void);

    /// @see https://docs.gtk.org/gtk4/signal.TextBuffer.redo.html
    DECLARE_SIGNAL(redo, "redo", void);

    /// @see https://docs.gtk.org/gtk4/signal.SelectionModel.selection-changed.html
    DECLARE_SIGNAL_MANUAL(selection_changed, "selection-changed", void,
        SPLAT(int32_t position, int32_t n_items),
        SPLAT(position, n_items)
    );

    using ModifierState = GdkModifierType;
    using KeyValue = guint;
    using KeyCode = guint;

    /// @see https://docs.gtk.org/gtk4/signal.EventControllerKey.key-pressed.html
    DECLARE_SIGNAL_MANUAL(key_pressed, "key-pressed", bool,
        SPLAT(KeyValue keyval, KeyCode keycode, ModifierState modifier),
        SPLAT(keyval, keycode, modifier)
    );

    /// @see https://docs.gtk.org/gtk4/signal.EventControllerKey.key-released.html
    DECLARE_SIGNAL_MANUAL(key_released, "key-released", void,
        SPLAT(KeyValue keyval, KeyCode keycode, ModifierState modifier),
        SPLAT(keyval, keycode, modifier)
    );

    /// @see https://docs.gtk.org/gtk4/signal.EventControllerKey.key-pressed.html
    DECLARE_SIGNAL_MANUAL(modifiers_changed, "modifiers", bool,
        SPLAT(KeyValue keyval, KeyCode keycode, ModifierState modifier),
        SPLAT(keyval, keycode, modifier)
    );

    /// @see https://docs.gtk.org/gtk4/signal.EventControllerMotion.enter.html
    DECLARE_SIGNAL_MANUAL(motion_enter, "enter", void,
        SPLAT(double x, double y),
        SPLAT(x, y)
    );

    /// @see https://docs.gtk.org/gtk4/signal.EventControllerMotion.motion.html
    DECLARE_SIGNAL_MANUAL(motion, "motion", void,
        SPLAT(double x, double y),
        SPLAT(x, y)
    );

    /// @see https://docs.gtk.org/gtk4/signal.EventControllerMotion.leave.html
    DECLARE_SIGNAL_MANUAL(motion_leave, "leave", void,
        SPLAT(double x, double y),
        SPLAT(x, y)
    );
}
