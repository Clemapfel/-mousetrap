//
// Copyright (c) Clemens Cords (mail@clemens-cords.com), created 4/11/23
//

#pragma once

#include <include/event_controller.hpp>

namespace mousetrap
{
    /// @brief type of stylus tool
    enum class ToolType
    {
        /// @brief unknown
        UNKNOWN = GDK_DEVICE_TOOL_TYPE_UNKNOWN,

        /// @brief tablet stylus
        PEN = GDK_DEVICE_TOOL_TYPE_UNKNOWN,

        /// @brief tablet eraser
        ERASER = GDK_DEVICE_TOOL_TYPE_UNKNOWN,

        /// @brief brush stylus
        BRUSH = GDK_DEVICE_TOOL_TYPE_UNKNOWN,

        /// @brief pencil stylus
        PENCIL = GDK_DEVICE_TOOL_TYPE_UNKNOWN,

        /// @brief airbrush tool
        AIRBRUSH = GDK_DEVICE_TOOL_TYPE_UNKNOWN,

        /// @brief tool is a lens cursor
        LENS = GDK_DEVICE_TOOL_TYPE_UNKNOWN,

        /// @brief tool is actually a mouse
        MOUSE = GDK_DEVICE_TOOL_TYPE_UNKNOWN
    };


    /// @brief handles events emitted by a touchpad stylus
    /// @todo expose high resolution motion history: https://docs.gtk.org/gtk4/method.GestureStylus.get_backlog.html
    class StylusEventController : public EventController,
        HAS_SIGNAL(StylusEventController, stylus_down),
        HAS_SIGNAL(StylusEventController, stylus_up),
        HAS_SIGNAL(StylusEventController, proximity),
        HAS_SIGNAL(StylusEventController, motion)
    {
        public:
            /// @brief construct
            StylusEventController();

            /// @brief get hardware id of stylus
            /// @return id, or 0 if ID is unknown
            size_t get_hardware_id() const;

            /// @brief get current tool type the stylus
            /// @return tool type
            ToolType get_device_type() const;
    };
}
