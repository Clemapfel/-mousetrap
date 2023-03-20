//
// Copyright 2022 Clemens Cords
// Created on 8/13/22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <include/widget.hpp>
#include <include/adjustment.hpp>

namespace mousetrap
{
    enum class ScrollbarVisibilityPolicy
    {
        NEVER = GTK_POLICY_NEVER,
        ALWAYS = GTK_POLICY_ALWAYS,
        AUTOMATIC = GTK_POLICY_AUTOMATIC,
        EXTERNAL = GTK_POLICY_EXTERNAL
    };

    enum class CornerPlacement
    {
        TOP_LEFT = GTK_CORNER_TOP_LEFT,
        TOP_RIGHT = GTK_CORNER_TOP_RIGHT,
        BOTTOM_LEFT = GTK_CORNER_BOTTOM_LEFT,
        BOTTOM_RIGHT = GTK_CORNER_BOTTOM_RIGHT
    };

    class ScrolledWindow : public WidgetImplementation<GtkScrolledWindow>
    {
        public:
            ScrolledWindow();
            ~ScrolledWindow();

            void set_propagate_natural_height(bool);
            bool get_propagate_natural_height() const;

            void set_propagate_natural_width(bool);
            bool get_propagate_natural_width() const;

            void set_horizontal_scrollbar_policy(ScrollbarVisibilityPolicy);
            ScrollbarVisibilityPolicy get_horizontal_scrollbar_policy() const;

            void set_vertical_scrollbar_policy(ScrollbarVisibilityPolicy);
            ScrollbarVisibilityPolicy get_vertical_scrollbar_policy() const;

            void set_scrollbar_placement(CornerPlacement content_relative_to_scrollbars);
            CornerPlacement get_scrollbar_placement() const;

            void set_has_frame(bool);
            bool get_has_frame() const;

            Adjustment& get_horizontal_adjustment();
            const Adjustment& get_horizontal_adjustment() const;

            Adjustment& get_vertical_adjustment();
            const Adjustment& get_vertical_adjustment() const;

            void set_kinetic_scrolling_enabled(bool);
            bool get_kinetic_scrolling_enabled() const;

            void set_child(Widget*);
            Widget* get_child() const;

        private:
            Widget* _child = nullptr;
            Adjustment* _h_adjustment = nullptr;
            Adjustment* _v_adjustment = nullptr;
    };
}


