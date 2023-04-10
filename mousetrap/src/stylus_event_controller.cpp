//
// Created by clem on 4/11/23.
//

#include <include/stylus_event_controller.hpp>

namespace mousetrap
{
    StylusEventController::StylusEventController()
        : EventController(GTK_EVENT_CONTROLLER(gtk_gesture_stylus_new())),
        CTOR_SIGNAL(StylusEventController, stylus_down),
        CTOR_SIGNAL(StylusEventController, stylus_up),
        CTOR_SIGNAL(StylusEventController, proximity),
        CTOR_SIGNAL(StylusEventController, motion)
    {}

    size_t StylusEventController::get_hardware_id() const
    {
        auto* device = gtk_gesture_stylus_get_device_tool(GTK_GESTURE_STYLUS(_native));
        if (device == nullptr)
            return 0;

        gdk_device_tool_get_hardware_id(device);
    }

    ToolType StylusEventController::get_device_type() const
    {
        auto* device = gtk_gesture_stylus_get_device_tool(GTK_GESTURE_STYLUS(_native));
        if (device == nullptr)
            return ToolType::UNKNOWN;

        return (ToolType) gdk_device_tool_get_tool_type(device);
    }
}