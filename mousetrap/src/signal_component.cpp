//
// Created by clem on 4/9/23.
//

#include <include/signal_component.hpp>
#include <include/log.hpp>

#include <iostream>

namespace mousetrap::detail
{
    #define DEFINE_SIGNAL(CamelCase, snake_case, CAPS_CASE, g_signal_id, return_t) \
        DECLARE_NEW_TYPE(CamelCase, snake_case, CAPS_CASE)                         \
                                                                                   \
        static void snake_case##_finalize(GObject* object) \
        {                                                                                                        \
            auto* self = MOUSETRAP_##CAPS_CASE(object);                         \
            G_OBJECT_CLASS(snake_case##_parent_class)->finalize(object);       \
        }                                                                           \
        \
        DEFINE_NEW_TYPE_TRIVIAL_INIT(CamelCase, snake_case, CAPS_CASE) \
        DEFINE_NEW_TYPE_TRIVIAL_CLASS_INIT(CamelCase, snake_case, CAPS_CASE) \
        \
        CamelCase* snake_case##_new(void* instance) \
        { \
            auto* self = (CamelCase*) g_object_new(snake_case##_get_type(), nullptr); \
            snake_case##_init(self); \
            self->instance = instance; \
            self->function = nullptr; \
            self->blocked = false; \
            return self; \
        }

    DEFINE_SIGNAL(Activate, activate, ACTIVATE, "activate", void);
    DEFINE_SIGNAL(Startup, startup, STARTUP, "startup", void);
    DEFINE_SIGNAL(Shutdown, shutdown, SHUTDOWN, "shutdown", void);
    DEFINE_SIGNAL(Update, update, UPDATE, "update", void);
    DEFINE_SIGNAL(Paint, paint, PAINT, "paint", void);
    DEFINE_SIGNAL(Realize, realize, REALIZE, "realize", void);
    DEFINE_SIGNAL(Unrealize, unrealize, UNREALIZE, "unrealize", void);
    DEFINE_SIGNAL(Destroy, destroy, DESTROY, "destroy", void);
    DEFINE_SIGNAL(Hide, hide, HIDE, "hide", void);
    DEFINE_SIGNAL(Show, show, SHOW, "show", void);
    DEFINE_SIGNAL(Map, map, MAP, "map", void);
    DEFINE_SIGNAL(Unmap, unmap, UNMAP, "unmap", void);
    DEFINE_SIGNAL(CloseRequest, close_request, CLOSE_REQUEST, "close-request", bool);
    DEFINE_SIGNAL(ActivateDefaultWidget, activate_default_widget, ACTIVATE_DEFAULT_WIDGET, "activate-default", void);
    DEFINE_SIGNAL(ActivateFocusWidget, activate_focused_widget, ACTIVATE_FOCUS_WIDGET, "activate-focus", void);
    DEFINE_SIGNAL(Clicked, clicked, CLICKED, "clicked", void);
    DEFINE_SIGNAL(Toggled, toggled, TOGGLED, "toggled", void);
    DEFINE_SIGNAL(TextChanged, text_changed, TEXT_CHANGED, "changed", void);
    DEFINE_SIGNAL(Undo, undo, UNDO, "undo", void);
    DEFINE_SIGNAL(Redo, redo, REDO, "redo", void);
    DEFINE_SIGNAL(MotionLeave, motion_leave, MOTION_LEAVE, "leave", void);
    DEFINE_SIGNAL(ClickStopped, click_stopped, CLICK_STOPPED, "stopped", void);
    DEFINE_SIGNAL(ScrollBegin, scroll_begin, SCROLL_BEGIN, "scroll-begin", void);
    DEFINE_SIGNAL(ScrollEnd, scroll_end, SCROLL_END, "scroll-end", void);
    DEFINE_SIGNAL(FocusGained, focus_gained, FOCUS_GAINED, "enter", void);
    DEFINE_SIGNAL(FocusLost, focus_lost, FOCUS_LOST, "leave", void);
    DEFINE_SIGNAL(PropertiesChanged, properties_changed, PROPERTIES_CHANGED, "changed", void);
    DEFINE_SIGNAL(ValueChanged, value_changed, VALUE_CHANGED, "value-changed", void);
    DEFINE_SIGNAL(Wrapped, wrapped, WRAPPED, "wrapped", void);


}
