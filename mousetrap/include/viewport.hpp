//
// Copyright 2022 Clemens Cords
// Created on 9/28/22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <include/widget.hpp>
#include <include/adjustment.hpp>

namespace mousetrap
{
    class Viewport : public WidgetImplementation<GtkViewport>
    {
        public:
            Viewport();
            void set_child(Widget* widget);

            Adjustment& get_horizontal_adjustment();
            const Adjustment& get_horizontal_adjustment() const;

            Adjustment& get_vertical_adjustment();
            const Adjustment& get_vertical_adjustment() const;

        private:
            Adjustment* _h_adjustment = nullptr;
            Adjustment* _v_adjustment = nullptr;
    };
}


