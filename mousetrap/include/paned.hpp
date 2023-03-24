//
// Copyright 2022 Clemens Cords
// Created on 9/14/22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <include/widget.hpp>
#include <include/orientable.hpp>

namespace mousetrap
{
    class Paned : public WidgetImplementation<GtkPaned>, public Orientable
    {
        public:
            Paned(Orientation);

            int get_position();
            void set_position(int);

            void set_start_child(Widget*);
            Widget* get_start_child() const;

            void set_start_child_resizable(bool);
            bool get_start_child_resizable() const;

            void set_start_child_shrinkable(bool);
            bool get_start_child_shrinkable() const;

            void set_end_child(Widget*);
            Widget* get_end_child() const;

            void set_end_child_resizable(bool);
            bool get_end_child_resizable() const;

            void set_end_child_shrinkable(bool);
            bool get_end_child_shrinkable() const;

            void set_has_wide_handle(bool);
            bool get_has_widget_handle() const;

            void set_orientation(Orientation) override;
            Orientation get_orientation() const override;

        private:
            Widget* _start_child = nullptr;
            Widget* _end_child = nullptr;
    };
}


