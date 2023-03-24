//
// Copyright 2022 Clemens Cords
// Created on 8/9/22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <include/widget.hpp>

namespace mousetrap
{
    /// @brief draws frame around widget, rounded corners. Has exactly one child and an optional label widget
    class Frame : public WidgetImplementation<GtkFrame>
    {
        public:
            /// @brief construct
            Frame();

            /// @brief set the child
            /// @param child
            void set_child(Widget*);

            /// @brief get child
            /// @return child
            Widget* get_child() const;

            /// @brief set label widget
            /// @param widget
            void set_label_widget(Widget*);

            /// @brief get label widget
            /// @return widget
            Widget* get_label_widget() const;

            /// @brief set label widget horizontal alignment
            /// @param x_align float in [0, 1], 0 for left-most, 1 for right-most alignment
            void set_label_x_alignment(float x_align);

            /// @brief get label horizontal alignment
            /// @return float in [0, 1], 0 for left-most, 1 for right-most alignment
            float get_label_x_alignment() const;

        private:
            Widget* _child = nullptr;
            Widget* _label_widget = nullptr;
    };
}
