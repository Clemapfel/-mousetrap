//
// Copyright 2022 Clemens Cords
// Created on 10/26/22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <include/widget.hpp>
#include <include/signal_component.hpp>

namespace mousetrap
{
    class Switch : public WidgetImplementation<GtkSwitch>,
        HAS_SIGNAL(Switch, activate)
    {
        public:
            Switch();

            bool get_active() const;
            void set_active(bool);
    };
}


