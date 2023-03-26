//
// Created by clem on 3/20/23.
//

#include <include/viewport.hpp>

namespace mousetrap
{
    Viewport::Viewport()
        : WidgetImplementation<GtkViewport>(GTK_VIEWPORT(gtk_viewport_new(nullptr, nullptr)))
    {
        _h_adjustment = new Adjustment(gtk_scrollable_get_hadjustment(GTK_SCROLLABLE(get_native())));
        _v_adjustment = new Adjustment(gtk_scrollable_get_vadjustment(GTK_SCROLLABLE(get_native())));
    }

    void Viewport::set_child(Widget* child)
    {
        _child = child;
        gtk_viewport_set_child(get_native(), _child == nullptr ? nullptr : _child->operator GtkWidget*());
    }

    Widget* Viewport::get_child()
    {
        return _child;
    }

    Adjustment& Viewport::get_horizontal_adjustment()
    {
        return *_h_adjustment;
    }

    Adjustment& Viewport::get_vertical_adjustment()
    {
        return *_v_adjustment;
    }

    const Adjustment& Viewport::get_horizontal_adjustment() const
    {
        return *_h_adjustment;
    }

    const Adjustment& Viewport::get_vertical_adjustment() const
    {
        return *_v_adjustment;
    }
}