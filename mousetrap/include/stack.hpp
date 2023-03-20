//
// Copyright 2022 Clemens Cords
// Created on 8/13/22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <include/widget.hpp>
#include <include/time.hpp>
#include <include/transition_type.hpp>
#include <include/selection_model.hpp>

namespace mousetrap
{
    using StackID = std::string;

    class Stack : public WidgetImplementation<GtkStack>
    {
        public:
            Stack();
            ~Stack();

            SelectionModel* get_selection_model();

            [[nodiscard]] StackID add_child(Widget*, const std::string& child_title);
            Widget* get_child(StackID);
            void remove_child(StackID);

            StackID get_visible_child();
            void set_visible_child(StackID);

            void set_transition_duration(Time);
            Time get_transition_duration() const;

            void set_transition_type(StackTransitionType);
            StackTransitionType get_transition_type() const;

            void set_is_horizontally_homogeneous(bool);
            bool get_is_horizontally_homogenous() const;

            void set_is_vertically_homogeneous(bool);
            bool get_is_vertically_homogenous() const;

            void set_should_interpolate_size(bool);
            bool get_should_interpolate_size() const;

        private:
            SelectionModel* _selection_model = nullptr;
            std::map<StackID, Widget*> _children;
    };

    class StackSidebar : public WidgetImplementation<GtkStackSidebar>
    {
        public:
            StackSidebar(Stack*);
    };

    class StackSwitcher : public WidgetImplementation<GtkStackSwitcher>
    {
        public:
            StackSwitcher(Stack*);
    };
}

