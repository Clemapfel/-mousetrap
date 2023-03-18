//
// Copyright 2022 Clemens Cords
// Created on 8/9/22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <include/widget.hpp>

namespace mousetrap
{
    class Frame : public WidgetImplementation<GtkFrame>
    {
        public:
            Frame();

            void set_child(Widget*);
            Widget* get_child() const;

            void set_label_widget(Widget*);
            Widget* get_label_widget() const;

            void set_label_x_alignment(float x_align);
            float get_label_x_alignment() const;

        private:
            Widget* _child = nullptr;
            Widget* _label_widget = nullptr;
    };
}
