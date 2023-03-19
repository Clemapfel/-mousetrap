//
// Copyright (c) Clemens Cords (mail@clemens-cords.com), created 3/19/23
//

#pragma once

#include <include/event_controller.hpp>
#include <include/vector.hpp>

namespace mousetrap
{
    class DragEventController : public EventController,
        HAS_SIGNAL(DragEventController, drag_begin),
        HAS_SIGNAL(DragEventController, drag),
        HAS_SIGNAL(DragEventController, drag_end)
    {
        public:
            DragEventController();

            Vector2f get_start_position();
            Vector2f get_current_offset();
    };
}
