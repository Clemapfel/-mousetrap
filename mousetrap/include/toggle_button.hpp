//
// Copyright 2022 Clemens Cords
// Created on 8/1/22 by clem (mail@clemens-cords.com)
//

#pragma once

#include "widget.hpp"
#include "action.hpp"

namespace mousetrap
{
    class ToggleButton : public WidgetImplementation<GtkToggleButton>,
        HAS_SIGNAL(ToggleButton, toggled)
    {
        public:
            ToggleButton();

            bool get_active() const;
            void set_active(bool b);

            void set_child(Widget*);
            Widget* get_child() const;

            void set_has_frame(bool b);
            bool get_has_frame() const;

        private:
            Widget* _child = nullptr;
    };
}

