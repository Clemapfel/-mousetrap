//
// Copyright 2022 Clemens Cords
// Created on 9/13/22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <include/widget.hpp>

namespace mousetrap
{
    class Expander : public WidgetImplementation<GtkExpander>,
        HAS_SIGNAL(Expander, activate)
    {
        public:
            Expander();

            void set_child(Widget*);
            Widget* get_child() const;

            void set_label_widget(Widget*);
            Widget* get_label_widget() const;

            bool get_expanded();
            void set_expanded(bool);

        private:
            Widget* _child = nullptr;
            Widget* _label_widget = nullptr;
    };
}
