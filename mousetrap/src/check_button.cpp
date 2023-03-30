//
// Copyright 2022 Clemens Cords
// Created on 9/16/22 by clem (mail@clemens-cords.com)
//

#include <include/check_button.hpp>

namespace mousetrap
{
    CheckButton::CheckButton()
        : WidgetImplementation<GtkCheckButton>(GTK_CHECK_BUTTON(gtk_check_button_new())),
          CTOR_SIGNAL(CheckButton, activate),
          CTOR_SIGNAL(CheckButton, toggled)
    {}

    void CheckButton::set_child(Widget* child)
    {
        _child = child;
        gtk_check_button_set_child(get_native(), child->operator GtkWidget*());
    }

    Widget* CheckButton::get_child() const
    {
        return _child;
    }

    void CheckButton::set_state(CheckButtonState state)
    {
        switch (state)
        {
            case CheckButtonState::ACTIVE:
                gtk_check_button_set_active(get_native(), TRUE);
                return;
            case CheckButtonState::INACTIVE:
                gtk_check_button_set_active(get_native(), FALSE);
                return;
            case CheckButtonState::INCONSISTENT:
                gtk_check_button_set_inconsistent(get_native(), true);
        }
    }

    CheckButtonState CheckButton::get_state() const
    {
        if (gtk_check_button_get_inconsistent(get_native()))
            return CheckButtonState::INCONSISTENT;
        else
            return gtk_check_button_get_active(get_native()) ? CheckButtonState::ACTIVE : CheckButtonState::INACTIVE;
    }
}