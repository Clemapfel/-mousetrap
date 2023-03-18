//
// Copyright 2022 Clemens Cords
// Created on 8/25/22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <include/widget.hpp>

namespace mousetrap
{
    class AspectFrame : public WidgetImplementation<GtkAspectFrame>
    {
        public:
            AspectFrame(float ratio, float x_align = 0.5, float y_align = 0.5);

            void set_ratio(float);
            void set_child_x_alignment(float);
            void set_child_y_alignment(float);

            float get_ratio() const;
            float get_child_x_alignment() const;
            float get_child_y_alignment() const;

            void set_child(Widget*);
            void remove_child();
    };
}
