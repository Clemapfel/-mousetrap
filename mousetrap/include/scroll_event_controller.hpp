//
// Copyright (c) Clemens Cords (mail@clemens-cords.com), created 3/19/23
//

#pragma once

#include <include/event_controller.hpp>

namespace mousetrap
{
    // handles mouse scroll
    class ScrollEventController : public EventController,
        HAS_SIGNAL(ScrollEventController, kinetic_scroll_decelerate),
        HAS_SIGNAL(ScrollEventController, scroll_begin),
        HAS_SIGNAL(ScrollEventController, scroll),
        HAS_SIGNAL(ScrollEventController, scroll_end)
    {
        public:
            ScrollEventController(bool emit_vertical = true, bool emit_horizontal = true);
    };
}