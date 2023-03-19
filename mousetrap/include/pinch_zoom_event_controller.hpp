//
// Copyright (c) Clemens Cords (mail@clemens-cords.com), created 3/20/23
//

#pragma once

#include <include/event_controller.hpp>

namespace mousetrap
{
    class PinchZoomEventController : public EventController,
        HAS_SIGNAL(PinchZoomEventController, scale_changed)
    {
        public:
            PinchZoomEventController();
            float get_scale_delta();
    };
}