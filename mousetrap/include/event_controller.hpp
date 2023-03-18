//
// Copyright 2022 Clemens Cords
// Created on 8/26/22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <include/signal_emitter.hpp>
#include <include/signal_component.hpp>
#include <include/vector.hpp>

#include <functional>

namespace mousetrap
{
    enum class PropagationPhase
    {
        NONE = GTK_PHASE_NONE,
        CAPTURE = GTK_PHASE_CAPTURE,
        BUBBLE = GTK_PHASE_BUBBLE,
        TARGET = GTK_PHASE_TARGET
    };

    class EventController : public SignalEmitter
    {
        public:
            ~EventController();
            operator GObject*() override;
            operator GtkEventController*();

            void set_propagation_phase(PropagationPhase);

        protected:
            EventController(GtkEventController*);
            GtkEventController* _native;
    };

    class KeyEventController : public EventController,
        HAS_SIGNAL(KeyEventController, key_pressed),
        HAS_SIGNAL(KeyEventController, key_released),
        HAS_SIGNAL(KeyEventController, modifiers_changed)
    {
        public:
            KeyEventController();
            bool should_shortcut_trigger_trigger(const std::string& shortcut);
    };
}
