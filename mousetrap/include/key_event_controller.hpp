//
// Copyright (c) Clemens Cords (mail@clemens-cords.com), created 3/18/23
//

#pragma once

#include <include/event_controller.hpp>

namespace mousetrap
{
    class KeyEventController : public EventController,
        HAS_SIGNAL(KeyEventController, key_pressed),
        HAS_SIGNAL(KeyEventController, key_released),
        HAS_SIGNAL(KeyEventController, modifiers_changed)
    {
        public:
            KeyEventController();
            bool should_shortcut_trigger_trigger(const std::string& shortcut);
            static bool shift_pressed(ModifierState);
            static bool control_pressed(ModifierState);
            static bool alt_pressed(ModifierState);
            static bool mouse_button_01_pressed(ModifierState);
            static bool mouse_button_02_pressed(ModifierState);
    };
}
