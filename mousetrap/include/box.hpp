//
// Copyright 2022 Clemens Cords
// Created on 7/31/22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <include/widget.hpp>
#include <include/orientation.hpp>

namespace mousetrap
{
    class Box : public WidgetImplementation<GtkBox>
    {
        public:
            Box(Orientation);

            void push_back(Widget*);
            void push_front(Widget*);
            void insert_after(Widget* to_append, Widget* after);
            void remove(Widget*);
            void clear();

            void set_homogeneous(bool);
            bool get_homogeneous() const;

            void set_spacing(float);
            float get_spacing() const;

            Orientation get_orientation() const;

            size_t get_n_items();
    };
}
