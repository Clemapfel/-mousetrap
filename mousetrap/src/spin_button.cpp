//
// Created by clem on 3/22/23.
//

#include <include/spin_button.hpp>
#include <iostream>

namespace mousetrap
{
    SpinButton::SpinButton(float min, float max, float step)
    : WidgetImplementation<GtkSpinButton>(GTK_SPIN_BUTTON(gtk_spin_button_new_with_range(min, max, step))),
      CTOR_SIGNAL(SpinButton, value_changed),
      CTOR_SIGNAL(SpinButton, wrapped)
    {
        _adjustment = Adjustment(gtk_spin_button_get_adjustment(get_native()));
    }

    void SpinButton::set_n_digits(size_t n)
    {
        gtk_spin_button_set_digits(get_native(), n);
    }

    size_t SpinButton::get_n_digits() const
    {
        return gtk_spin_button_get_digits(get_native());
    }

    void SpinButton::set_should_wrap(bool b)
    {
        gtk_spin_button_set_wrap(get_native(), b);
    }

    bool SpinButton::get_should_wrap() const
    {
        return gtk_spin_button_get_wrap(get_native());
    }

    Adjustment& SpinButton::get_adjustment()
    {
        return _adjustment;
    }

    const Adjustment& SpinButton::get_adjustment() const
    {
        return _adjustment;
    }

    void SpinButton::set_acceleration_rate(float v)
    {
        gtk_spin_button_set_climb_rate(get_native(), v);
    }

    float SpinButton::get_acceleration_rate() const
    {
        return gtk_spin_button_get_climb_rate(get_native());
    }

    void SpinButton::set_should_snap_to_ticks(bool b)
    {
        gtk_spin_button_set_snap_to_ticks(get_native(), b);
    }

    bool SpinButton::get_should_snap_to_ticks() const
    {
        return gtk_spin_button_get_snap_to_ticks(get_native());
    }

    void SpinButton::set_allow_only_numeric(bool b)
    {
        gtk_spin_button_set_numeric(get_native(), b);
    }

    bool SpinButton::get_allow_only_numeric() const
    {
        return gtk_spin_button_get_numeric(get_native());
    }

    float SpinButton::get_lower() const
    {
        return gtk_adjustment_get_lower(gtk_spin_button_get_adjustment(get_native()));
    }

    float SpinButton::get_increment() const
    {
        return gtk_adjustment_get_minimum_increment(gtk_spin_button_get_adjustment(get_native()));
    }

    float SpinButton::get_upper() const
    {
        return gtk_adjustment_get_upper(gtk_spin_button_get_adjustment(get_native()));
    }

    float SpinButton::get_value() const
    {
        return gtk_adjustment_get_value(gtk_spin_button_get_adjustment(get_native()));
    }

    void SpinButton::set_value(float value)
    {
        gtk_adjustment_set_value(gtk_spin_button_get_adjustment(get_native()), value);
    }

    void SpinButton::set_lower(float value)
    {
        gtk_adjustment_set_lower(gtk_spin_button_get_adjustment(get_native()), value);
    }

    void SpinButton::set_upper(float value)
    {
        gtk_adjustment_set_upper(gtk_spin_button_get_adjustment(get_native()), value);
    }

    void SpinButton::set_increment(float value)
    {
        gtk_adjustment_set_step_increment(gtk_spin_button_get_adjustment(get_native()), value);
    }

    void SpinButton::reset_text_to_value_function()
    {
        disconnect_signal("input");
    }

    void SpinButton::reset_value_to_text_function()
    {
        disconnect_signal("output");
    }

    gint SpinButton::on_input(GtkSpinButton* button, double* new_value, SpinButton* instance)
    {
        std::string text = gtk_editable_get_text(GTK_EDITABLE(button));
        float res = 0;
        try
        {
            res = instance->_text_to_value_f(instance, text);
        }
        catch (...)
        {
            return GTK_INPUT_ERROR;
        }

        *new_value = (double) res;
        return TRUE;
    }

    bool SpinButton::on_output(GtkSpinButton* button, SpinButton* instance)
    {
        float value = gtk_adjustment_get_value(gtk_spin_button_get_adjustment(button));
        std::string res = instance->_value_to_text_f(instance, value);
        gtk_editable_set_text(GTK_EDITABLE(button), res.c_str());

        return TRUE;
    }
}
