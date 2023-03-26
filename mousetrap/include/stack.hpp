//
// Copyright 2022 Clemens Cords
// Created on 8/13/22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <include/widget.hpp>
#include <include/time.hpp>
#include <include/transition_type.hpp>
#include <include/selection_model.hpp>
#include <include/selectable.hpp>

namespace mousetrap
{
    class Stack : public WidgetImplementation<GtkStack>, public Selectable
    {
        using ID = std::string;
            
        public:
            Stack();
            ~Stack();

            SelectionModel* get_selection_model() override;

            [[nodiscard]] Stack::ID add_child(Widget*, const std::string& child_title);
            Widget* get_child(Stack::ID);
            void remove_child(Stack::ID);

            Stack::ID get_visible_child();
            void set_visible_child(Stack::ID);

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
            std::map<Stack::ID, Widget*> _children;
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

