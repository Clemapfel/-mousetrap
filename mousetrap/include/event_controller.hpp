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
    /// @brief event controller propagation phase, decides at what part of the event loop the handler is invoked
    /// @see https://docs.gtk.org/gtk4/enum.PropagationPhase.html
    enum class PropagationPhase
    {
        /// @brief is never triggered
        NONE = GTK_PHASE_NONE,

        /// @brief is triggered during event propagation down from the toplevel window towards the inner-most widget
        CAPTURE = GTK_PHASE_CAPTURE,

        /// @brief is triggered during event propagation up from the inner most widget to the toplevel window
        BUBBLE = GTK_PHASE_BUBBLE,

        /// @brief triggered only when containing widget specifically pulls an event
        TARGET = GTK_PHASE_TARGET
    };

    /// @brief event controller
    class EventController : public SignalEmitter
    {
        public:
            /// @brief default ctor delete, cannot be instantiated. Use one of its base classes instead
            EventController() = delete;

            /// @brief dtor
            ~EventController();

            /// @brief expose as GObject \internal
            operator GObject*() const override;

            /// @brief expose as GtkEventController \internal
            operator GtkEventController*() const;

            /// @brief set propagation phase
            /// @param phase
            void set_propagation_phase(PropagationPhase);

            /// @brief get propagation phase
            /// @return phase
            PropagationPhase get_propagation_phase() const;

        protected:
            EventController(GtkEventController*);
            GtkEventController* _native;
    };
}
