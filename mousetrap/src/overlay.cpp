//
// Copyright 2022 Clemens Cords
// Created on 8/1/22 by clem (mail@clemens-cords.com)
//

#include <include/overlay.hpp>

namespace mousetrap
{
    Overlay::Overlay()
        : WidgetImplementation<GtkOverlay>(GTK_OVERLAY(gtk_overlay_new()))
    {}

    void Overlay::set_child(Widget* in)
    {
        _child = in;
        gtk_overlay_set_child(get_native(), _child == nullptr ? nullptr : _child->operator GtkWidget*());
    }

    void Overlay::add_overlay(Widget* widget, bool included_in_measurement, bool clip)
    {
        auto* gtk_widget = widget != nullptr ? widget->operator GtkWidget*() : nullptr;
        if (gtk_widget == nullptr)
            return;

        gtk_overlay_add_overlay(get_native(), gtk_widget);
        gtk_overlay_set_measure_overlay(get_native(), gtk_widget, included_in_measurement);
        gtk_overlay_set_clip_overlay(get_native(), gtk_widget, clip);
    }

    void Overlay::remove_overlay(Widget* in)
    {
        if (in == nullptr)
            return;

        gtk_overlay_remove_overlay(get_native(), in->operator GtkWidget*());
    }

    void Overlay::remove_child()
    {
        gtk_overlay_set_child(get_native(), nullptr);
    }

    Widget* Overlay::get_child() const
    {
        return _child;
    }
}