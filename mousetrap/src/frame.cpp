//
// Copyright 2022 Clemens Cords
// Created on 8/9/22 by clem (mail@clemens-cords.com)
//

#include <include/frame.hpp>
#include <include/log.hpp>

namespace mousetrap
{
    Frame::Frame()
        : WidgetImplementation<GtkFrame>(GTK_FRAME(gtk_frame_new("")))
    {
        set_label_widget(nullptr);
    }

    void Frame::set_child(Widget* in)
    {
        WARN_IF_SELF_INSERTION(Frame::set_child, this, in);

        _child = in;
        gtk_frame_set_child(get_native(), _child == nullptr ? nullptr : _child->operator GtkWidget *());
    }

    Widget* Frame::get_child() const
    {
        return _child;
    }

    void Frame::set_label_widget(Widget* widget)
    {
        WARN_IF_SELF_INSERTION(Frame::set_label_widget, this, widget);

        _label_widget = widget;
        gtk_frame_set_label_widget(get_native(), _label_widget == nullptr ? nullptr : _label_widget->operator GtkWidget*());
    }

    Widget* Frame::get_label_widget() const
    {
        return _label_widget;
    }

    void Frame::set_label_x_alignment(float x)
    {
        gtk_frame_set_label_align(get_native(), x);
    }

    float Frame::get_label_x_alignment() const
    {
        return gtk_frame_get_label_align(get_native());
    }
}