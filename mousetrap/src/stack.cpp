//
// Copyright 2022 Clemens Cords
// Created on 8/13/22 by clem (mail@clemens-cords.com)
//

#include <include/stack.hpp>
#include <include/log.hpp>

#include <iostream>

namespace mousetrap
{
    StackSidebar::StackSidebar(const Stack& stack)
        : WidgetImplementation<GtkStackSidebar>(GTK_STACK_SIDEBAR(gtk_stack_sidebar_new()))
    {
        gtk_stack_sidebar_set_stack(get_native(), stack.operator GtkStack*());
    }

    StackSwitcher::StackSwitcher(const Stack& stack)
        : WidgetImplementation<GtkStackSwitcher>(GTK_STACK_SWITCHER(gtk_stack_switcher_new()))
    {
        gtk_stack_switcher_set_stack(get_native(), GTK_STACK(stack.operator GtkStack*()));
    }

    Stack::Stack()
        : WidgetImplementation<GtkStack>(GTK_STACK(gtk_stack_new()))
    {
    }

    Stack::~Stack()
    {
        delete _selection_model;
    }

    SelectionModel* Stack::get_selection_model()
    {
        if (_selection_model == nullptr)
            _selection_model = new SelectionModel(gtk_stack_get_pages(get_native()));

        return _selection_model;
    }

    Stack::ID Stack::add_child(Widget* widget, const std::string& title)
    {
        if (widget != nullptr and widget->operator GtkWidget*() == this->operator GtkWidget*())
        {
            log::critical("In Stack::add_child: Attempting to insert Stack into itself. This would cause an infinite loop");
            widget = nullptr;
        }

        gtk_stack_add_titled(get_native(), widget == nullptr ? nullptr : widget->operator GtkWidget*(), title.c_str(), title.c_str());
        _children.insert({title, widget});
        return title;
    }

    void Stack::remove_child(Stack::ID id)
    {
        if (_children.find(id) == _children.end())
        {
            std::stringstream str;
            str << "In Stack::remove_child: No child with ID `" << id << "`";
            log::critical(str.str(), MOUSETRAP_DOMAIN);
            return;
        }

        gtk_stack_remove(get_native(), _children.at(id)->operator GtkWidget *());
        _children.erase(id);
    }

    Stack::ID Stack::get_visible_child()
    {
        return gtk_stack_get_visible_child_name(get_native());
    }

    void Stack::set_visible_child(Stack::ID id)
    {
        gtk_stack_set_visible_child_name(get_native(), id.c_str());
    }

    void Stack::set_transition_duration(Time time)
    {
        gtk_stack_set_transition_duration(get_native(), time.as_milliseconds());
    }

    Time Stack::get_transition_duration() const
    {
        return milliseconds(gtk_stack_get_transition_duration(get_native()));
    }

    void Stack::set_transition_type(StackTransitionType type)
    {
        gtk_stack_set_transition_type(get_native(), (GtkStackTransitionType) type);
    }

    StackTransitionType Stack::get_transition_type() const
    {
        return (StackTransitionType) gtk_stack_get_transition_type(get_native());
    }

    void Stack::set_is_horizontally_homogeneous(bool b)
    {
        gtk_stack_set_hhomogeneous(get_native(), b);
    }

    bool Stack::get_is_horizontally_homogenous() const
    {
        return gtk_stack_get_hhomogeneous(get_native());
    }

    void Stack::set_is_vertically_homogeneous(bool b)
    {
        gtk_stack_set_vhomogeneous(get_native(), b);
    }

    bool Stack::get_is_vertically_homogenous() const
    {
        return gtk_stack_get_vhomogeneous(get_native());
    }

    void Stack::set_should_interpolate_size(bool b)
    {
        gtk_stack_set_interpolate_size(get_native(), b);
    }

    bool Stack::get_should_interpolate_size() const
    {
        return gtk_stack_get_interpolate_size(get_native());
    }


}

