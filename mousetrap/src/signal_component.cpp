



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

    #define DEFINE_SIGNAL_MANUAL(CameCase, snake_case, CAPS_CASE, g_signal_id, return_t, ...) \
        DEFINE_SIGNAL(CamelCase, snake_case, CAPS_CASE, g_signal_id, return_t)

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
    DEFINE_SIGNAL_MANUAL(TextInserted, text_inserted, TEXT_INSERTED, "insert-text", void,
                              SPLAT(int32_t start_pos, int32_t end_pos),
                              SPLAT(start_pos, end_pos)
        );

    DEFINE_SIGNAL_MANUAL(TextDeleted, text_deleted, TEXT_DELETED, "delete-text", void,
                          SPLAT(int32_t start_pos, int32_t end_pos),
                          SPLAT(start_pos, end_pos)
    );

    DEFINE_SIGNAL(Undo, undo, UNDO, "undo", void);
    DEFINE_SIGNAL(Redo, redo, REDO, "redo", void);
    DEFINE_SIGNAL_MANUAL(SelectionChanged, selection_changed, SELECTION_CHANGED, "selection-changed", void,
                              SPLAT(int32_t position, int32_t n_items),
                              SPLAT(position, n_items)
        );
    

    DEFINE_SIGNAL_MANUAL(KeyPressed, key_pressed, KEY_PRESSED, "key-pressed", bool,
                          SPLAT(KeyValue keyval, KeyCode keycode, ModifierState modifier),
                          SPLAT(keyval, keycode, modifier)
    );


    DEFINE_SIGNAL_MANUAL(KeyReleased, key_released, KEY_RELEASED, "key-released", void,
                          SPLAT(KeyValue keyval, KeyCode keycode, ModifierState modifier),
                          SPLAT(keyval, keycode, modifier)
    );DEFINE_SIGNAL_MANUAL(ModifiersChanged, modifiers_changed, MODIFIERS_CHANGED, "modifiers", bool,
                          SPLAT(KeyValue keyval, KeyCode keycode, ModifierState modifier),
                          SPLAT(keyval, keycode, modifier)
    );


    DEFINE_SIGNAL_MANUAL(MotionEnter, motion_enter, MOTION_ENTER, "enter", void,
                          SPLAT(double x, double y),
                          SPLAT(x, y)
    );

    DEFINE_SIGNAL_MANUAL(Motion, motion, MOTION, "motion", void,
                          SPLAT(double x, double y),
                          SPLAT(x, y)
    );

    DEFINE_SIGNAL(MotionLeave, motion_leave, MOTION_LEAVE, "leave", void);
    DEFINE_SIGNAL_MANUAL(ClickPressed, click_pressed, CLICK_PRESSED, "pressed", void,
                          SPLAT(int32_t n_press, double x, double y),
                          SPLAT(n_press, x, y)
    );
    
    DEFINE_SIGNAL_MANUAL(ClickReleased, click_released, CLICK_RELEASED, "released", void,
                          SPLAT(int32_t n_press, double x, double y),
                          SPLAT(n_press, x, y)
    );


    DEFINE_SIGNAL(ClickStopped, click_stopped, CLICK_STOPPED, "stopped", void);
        DEFINE_SIGNAL_MANUAL(KineticScrollDecelerate, kinetic_scroll_decelerate, KINETIC_SCROLL_DECELERATE, "decelerate", void,
                          SPLAT(double x_velocity, double y_velocity),
                          SPLAT(x_velocity, y_velocity)
    );

    DEFINE_SIGNAL(ScrollBegin, scroll_begin, SCROLL_BEGIN, "scroll-begin", void);
DEFINE_SIGNAL_MANUAL(Scroll, scroll, SCROLL, "scroll", bool,
                          SPLAT(double delta_x, double delta_y),
                          SPLAT(delta_x, delta_y)
    );

    DEFINE_SIGNAL(ScrollEnd, scroll_end, SCROLL_END, "scroll-end", void);
    DEFINE_SIGNAL(FocusGained, focus_gained, FOCUS_GAINED, "enter", void);
    DEFINE_SIGNAL(FocusLost, focus_lost, FOCUS_LOST, "leave", void);
    DEFINE_SIGNAL_MANUAL(DragBegin, drag_begin, DRAG_BEGIN, "drag-begin", void,
                          SPLAT(double start_x, double start_y),
                          SPLAT(start_x, start_y)
    );

    DEFINE_SIGNAL_MANUAL(Drag, drag, DRAG, "drag-update", void,
                          SPLAT(double offset_x, double offset_y),
                          SPLAT(offset_x, offset_y)
    );

    DEFINE_SIGNAL_MANUAL(DragEnd, drag_end, DRAG_END, "drag-end", void,
                          SPLAT(double offset_x, double offest_y),
                          SPLAT(offset_x, offest_y)
    );

    DEFINE_SIGNAL_MANUAL(ScaleChanged, scale_changed, SCALE_CHANGED, "scale-changed", void,
                          double scale,
                          scale
    );
    DEFINE_SIGNAL_MANUAL(RotationChanged, rotation_changed, ROTATION_CHANGED, "angle-changed", void,
                          SPLAT(double angle_absolute_radians, double angle_delta_radians),
                          SPLAT(angle_absolute_radians, angle_delta_radians)
    );

    DEFINE_SIGNAL(PropertiesChanged, properties_changed, PROPERTIES_CHANGED, "changed", void);    
    DEFINE_SIGNAL(ValueChanged, value_changed, VALUE_CHANGED, "value-changed", void);
    DEFINE_SIGNAL_MANUAL(Render, render, RENDER, "render", bool, GdkGLContext* context, context);
    DEFINE_SIGNAL_MANUAL(Resize, resize, RESIZE, "resize", void,
                          SPLAT(int32_t width, int32_t height),
                          SPLAT(width, height)
    );

    DEFINE_SIGNAL_MANUAL(PageAdded, page_added, PAGE_ADDED, "page-added", void,
                          SPLAT(GtkWidget* _, uint32_t page_index),
                          SPLAT(_, page_index)
    );

    DEFINE_SIGNAL_MANUAL(PageRemoved, page_removed, PAGE_REMOVED, "page-removed", void,
                          SPLAT(GtkWidget* _, uint32_t page_index),
                          SPLAT(_, page_index)
    );

    DEFINE_SIGNAL_MANUAL(PageReordered, page_reordered, PAGE_REORDERED, "page-reordered", void,
                          SPLAT(GtkWidget* widget, uint32_t page_index),
                          SPLAT(widget, page_index)
    );

    DEFINE_SIGNAL_MANUAL(PageSelectionChanged, page_selection_changed, PAGE_SELECTION_CHANGED, "switch-page", void,
                          SPLAT(GtkWidget* _, uint32_t page_index),
                          SPLAT(_, page_index)
    );

    DEFINE_SIGNAL(Wrapped, wrapped, WRAPPED, "wrapped", void);
    DEFINE_SIGNAL_MANUAL(Response, response, RESPONSE, "response", void, int response, response);
}
