//
// Copyright (c) Clemens Cords (mail@clemens-cords.com), created 3/20/23
//

#pragma once

#include <include/event_controller.hpp>
#include <include/angle.hpp>

namespace mousetrap
{
    class RotateEventController : public EventController,
        HAS_SIGNAL(RotateEventController, rotation_changed)
    {
        public:
            RotateEventController();
            Angle get_angle_delta();
    };
}
