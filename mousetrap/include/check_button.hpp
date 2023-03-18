//
// Copyright 2022 Clemens Cords
// Created on 9/16/22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <include/widget.hpp>

namespace mousetrap
{
    enum class CheckButtonState
    {
        ACTIVE = 1,
        INACTIVE = 0,
        INCONSISTENT = -1
    };

    class CheckButton : public WidgetImplementation<GtkCheckButton>,
        HAS_SIGNAL(CheckButton, activate),
        HAS_SIGNAL(CheckButton, toggled)
    {
        public:
            CheckButton();

            void set_state(CheckButtonState);
            CheckButtonState get_state() const;

            void set_child(Widget*);
            Widget* get_child() const;

        private:
            Widget* _child = nullptr;
    };
}
