//
// Copyright 2022 Clemens Cords
// Created on 8/11/22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <gtk/gtk.h>
#include "signal_emitter.hpp"
#include "signal_component.hpp"

namespace mousetrap
{
    class Adjustment : public SignalEmitter,
        HAS_SIGNAL(Adjustment, value_changed),
        HAS_SIGNAL(Adjustment, properties_changed)
    {
        public:
            Adjustment();
            Adjustment(GtkAdjustment*);
            Adjustment(float current, float lower, float upper, float increment);

            ~Adjustment();

            Adjustment(const Adjustment&) = delete;
            Adjustment& operator=(const Adjustment&) = delete;

            Adjustment(Adjustment&&) noexcept;
            Adjustment& operator=(Adjustment&&) noexcept;

            operator GtkAdjustment*();
            operator GObject*() override;

            float get_lower() const;
            float get_upper() const;
            float get_value() const;
            float get_increment() const;

            void set_lower(float);
            void set_upper(float);
            void set_value(float);
            void set_step_increment(float);

        private:
            GtkAdjustment* _native = nullptr;
    };
}
