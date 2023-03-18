//
// Copyright 2022 Clemens Cords
// Created on 7/31/22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <include/widget.hpp>
#include <include/signal_component.hpp>
#include <include/action.hpp>

namespace mousetrap
{
    class Button : public WidgetImplementation<GtkButton>,
        HAS_SIGNAL(Button, activate),
        HAS_SIGNAL(Button, clicked)
    {
        public:
            Button();

            void set_has_frame(bool b);
            bool get_has_frame() const;

            void set_child(Widget*);
            Widget* get_child() const;

            void set_action(Action&);

        private:
            Widget* _child = nullptr;
    };
}
