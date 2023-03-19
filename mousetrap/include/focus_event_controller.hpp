//
// Copyright (c) Clemens Cords (mail@clemens-cords.com), created 3/19/23
//

#pragma once

#include <include/event_controller.hpp>

namespace mousetrap
{
    class FocusEventController : public EventController,
        HAS_SIGNAL(FocusEventController, focus_gained),
        HAS_SIGNAL(FocusEventController, focus_lost)
    {
        public:
            FocusEventController();

            /// @brief focus is contained within self or any child
            bool self_or_child_is_focused();

            /// @brief focus is contained within self and not any child
            bool self_is_focused();
    };
}