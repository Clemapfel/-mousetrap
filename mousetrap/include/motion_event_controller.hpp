//
// Copyright (c) Clemens Cords (mail@clemens-cords.com), created 3/18/23
//

#pragma once

#include <include/event_controller.hpp>

namespace mousetrap
{
    class MotionEventController : public EventController,
        HAS_SIGNAL(MotionEventController, motion_enter),
        HAS_SIGNAL(MotionEventController, motion),
        HAS_SIGNAL(MotionEventController, motion_leave)
    {
        public:
            MotionEventController();
    };
}