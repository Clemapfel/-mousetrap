//
// Created by clem on 3/19/23.
//

#include <include/click_event_controller.hpp>

namespace mousetrap
{
    ClickEventController::ClickEventController()
        : EventController(GTK_EVENT_CONTROLLER(gtk_gesture_click_new())),
          CTOR_SIGNAL(ClickEventController, click_pressed),
          CTOR_SIGNAL(ClickEventController, click_released),
          CTOR_SIGNAL(ClickEventController, click_stopped)
    {}
}
