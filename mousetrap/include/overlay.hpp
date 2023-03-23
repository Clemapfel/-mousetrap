//
// Copyright 2022 Clemens Cords
// Created on 7/31/22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <include/widget.hpp>

namespace mousetrap
{
    class Overlay : public WidgetImplementation<GtkOverlay>
    {
        public:
            Overlay();

            void set_child(Widget*);
            void remove_child();
            Widget* get_child();

            void add_overlay(Widget*, bool included_in_measurement = true, bool clip = false);
            void remove_overlay(Widget*);

        private:
            Widget* _child = nullptr;
    };
}


