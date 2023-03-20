//
// Copyright 2022 Clemens Cords
// Created on 9/3/22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <include/widget.hpp>
#include <include/relative_position.hpp>

namespace mousetrap
{
    class Popover : public WidgetImplementation<GtkPopover>
    {
        public:
            Popover();

            void popup();
            void popdown();
            void present();

            void set_child(Widget*);
            Widget* get_child() const;

            void set_relative_position(RelativePosition);
            RelativePosition get_relative_position() const;

            void set_autohide(bool);
            bool get_autohide() const;

            void set_has_base_arrow(bool);
            bool get_has_base_arrow() const;

            void attach_to(Widget*);

        private:
            Widget* _child = nullptr;
    };
}
