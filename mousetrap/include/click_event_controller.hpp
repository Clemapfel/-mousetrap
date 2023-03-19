//
// Copyright (c) Clemens Cords (mail@clemens-cords.com), created 3/19/23
//

#pragma once

#include <include/event_controller.hpp>

namespace mousetrap
{
    /// handles mouse button press
    class ClickEventController : public EventController,
        HAS_SIGNAL(ClickEventController, click_pressed),
        HAS_SIGNAL(ClickEventController, click_released),
        HAS_SIGNAL(ClickEventController, click_stopped)
    {
        public:
            ClickEventController();
    };
}