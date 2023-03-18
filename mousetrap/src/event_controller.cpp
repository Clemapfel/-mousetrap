//
// Created by clem on 3/18/23.
//


#include <include/event_controller.hpp>
#include <include/vector.hpp>

namespace mousetrap
{
    EventController::EventController(GtkEventController* controller)
    {
        _native = g_object_ref(controller);
    }

    EventController::~EventController()
    {
        g_object_unref(_native);
    }

    EventController::operator GObject*()
    {
        return G_OBJECT(_native);
    }

    EventController::operator GtkEventController*()
    {
        return _native;
    }

    void EventController::set_propagation_phase(PropagationPhase phase)
    {
        gtk_event_controller_set_propagation_phase(_native, (GtkPropagationPhase) phase);
    }

    KeyEventController::KeyEventController()
        : EventController(GTK_EVENT_CONTROLLER(gtk_event_controller_key_new())),
          CTOR_SIGNAL(KeyEventController, key_pressed),
          CTOR_SIGNAL(KeyEventController, key_released),
          CTOR_SIGNAL(KeyEventController, modifiers_changed)
    {}

    bool KeyEventController::should_shortcut_trigger_trigger(const std::string& shortcut)
    {
        auto* trigger = gtk_shortcut_trigger_parse_string(shortcut.c_str());
        auto* event = gtk_event_controller_get_current_event(_native);
        bool out = gtk_shortcut_trigger_trigger(trigger, event, false);
        g_object_unref(trigger);
        return out;
    };
}
