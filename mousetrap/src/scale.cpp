//
// Created by clem on 3/23/23.
//

#include <include/scale.hpp>
#include <iostream>

namespace mousetrap
{
    Scale::Scale(float min, float max, float step, Orientation orientation)
        : WidgetImplementation<GtkScale>(GTK_SCALE(gtk_scale_new_with_range((GtkOrientation) orientation, min, max, step))),
          CTOR_SIGNAL(Scale, value_changed)
    {
        gtk_scale_set_draw_value(get_native(), false);
        _adjustment = Adjustment(gtk_range_get_adjustment(GTK_RANGE(get_native())));
    }

    Adjustment& Scale::get_adjustment() 
    {
        return _adjustment;
    }

    const Adjustment& Scale::get_adjustment() const 
    {
        return _adjustment;
    }

    float Scale::get_lower() const
    {
        return gtk_adjustment_get_lower(gtk_range_get_adjustment(GTK_RANGE(get_native())));
    }

    float Scale::get_increment() const
    {
        return gtk_adjustment_get_minimum_increment(gtk_range_get_adjustment(GTK_RANGE(get_native())));
    }

    float Scale::get_upper() const
    {
        return gtk_adjustment_get_upper(gtk_range_get_adjustment(GTK_RANGE(get_native())));
    }

    float Scale::get_value() const
    {
        return gtk_adjustment_get_value(gtk_range_get_adjustment(GTK_RANGE(get_native())));
    }

    void Scale::set_value(float value)
    {
        gtk_adjustment_set_value(gtk_range_get_adjustment(GTK_RANGE(get_native())), value);
    }

    void Scale::set_lower(float value)
    {
        gtk_adjustment_set_lower(gtk_range_get_adjustment(GTK_RANGE(get_native())), value);
    }

    void Scale::set_upper(float value)
    {
        gtk_adjustment_set_upper(gtk_range_get_adjustment(GTK_RANGE(get_native())), value);
    }

    void Scale::set_increment(float value)
    {
        gtk_adjustment_set_step_increment(gtk_range_get_adjustment(GTK_RANGE(get_native())), value);
    }

    void Scale::set_should_draw_value(bool b)
    {
        gtk_scale_set_draw_value(get_native(), b);
    }

    bool Scale::get_should_draw_value() const
    {
        return gtk_scale_get_draw_value(get_native());
    }

    void Scale::set_has_origin(bool b)
    {
        gtk_scale_set_has_origin(get_native(), b);
    }

    bool Scale::get_has_origin() const
    {
        return gtk_scale_get_has_origin(get_native());
    }

    void Scale::add_mark(float at, RelativePosition pos, const std::string& label)
    {
        gtk_scale_add_mark(get_native(), at, (GtkPositionType) pos, label.c_str());
    }

    void Scale::clear_marks()
    {
        gtk_scale_clear_marks(get_native());
    }

    void Scale::reset_format_value_function()
    {
        gtk_scale_set_format_value_func(get_native(), nullptr, nullptr, nullptr);
    }

    char* Scale::on_format_value(GtkScale* scale, double value, Scale* instance)
    {
        auto out = instance->_format_f(value);
        return g_strdup(out.c_str());
    }

    void Scale::set_orientation(Orientation orientation)
    {
        gtk_orientable_set_orientation(GTK_ORIENTABLE(get_native()), (GtkOrientation) orientation);
    }

    Orientation Scale::get_orientation() const
    {
        return (Orientation) gtk_orientable_get_orientation(GTK_ORIENTABLE(get_native()));
    }
}
