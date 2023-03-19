//
// Copyright 2022 Clemens Cords
// Created on 8/1/22 by clem (mail@clemens-cords.com)
//

#include <include/button.hpp>

namespace mousetrap
{
    Button::Button()
    : WidgetImplementation<GtkButton>(GTK_BUTTON(gtk_button_new())),
      CTOR_SIGNAL(Button, activate),
      CTOR_SIGNAL(Button, clicked)
    {}

    void Button::set_has_frame(bool b)
    {
        gtk_button_set_has_frame(get_native(), b);
    }

    bool Button::get_has_frame() const
    {
        return gtk_button_get_has_frame(get_native());
    }

    void Button::set_child(Widget* widget)
    {
        _child = widget;
        gtk_button_set_child(get_native(), _child == nullptr ? nullptr : _child->operator GtkWidget*());
    }

    Widget* Button::get_child() const
    {
        return _child;
    }

    void Button::set_action(Action& action)
    {
        gtk_actionable_set_action_name(GTK_ACTIONABLE(get_native()), ("app."+ action.get_id()).c_str());
    }
}