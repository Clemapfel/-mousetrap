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

    PropagationPhase EventController::get_propagation_phase() const
    {
        return (GtkPropagationPhase) gtk_event_controller_get_propagation_phase(_native);
    }
}
