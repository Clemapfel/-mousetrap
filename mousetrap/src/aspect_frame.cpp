//
// Copyright 2022 Clemens Cords
// Created on 8/9/22 by clem (mail@clemens-cords.com)
//

#include <include/aspect_frame.hpp>

namespace mousetrap
{
    AspectFrame::AspectFrame(float ratio, float x_align, float y_align)
        : WidgetImplementation<GtkAspectFrame>(GTK_ASPECT_FRAME(gtk_aspect_frame_new(x_align, y_align, ratio, false)))
    {}

    void AspectFrame::set_child(Widget* child)
    {
        gtk_aspect_frame_set_child(get_native(), child == nullptr ? nullptr : child->operator GtkWidget*());
    }

    void AspectFrame::remove_child()
    {
        gtk_aspect_frame_set_child(get_native(), nullptr);
    }

    void AspectFrame::set_ratio(float new_ratio)
    {
        gtk_aspect_frame_set_ratio(get_native(), new_ratio);
    }

    void AspectFrame::set_child_x_alignment(float x)
    {
        gtk_aspect_frame_set_xalign(get_native(), x);
    }

    void AspectFrame::set_child_y_alignment(float x)
    {
        gtk_aspect_frame_set_yalign(get_native(), x);
    }

    float AspectFrame::get_ratio() const
    {
        return gtk_aspect_frame_get_ratio(get_native());
    }

    float AspectFrame::get_child_x_alignment() const
    {
        return gtk_aspect_frame_get_xalign(get_native());
    }

    float AspectFrame::get_child_y_alignment() const
    {
        return gtk_aspect_frame_get_yalign(get_native());
    }
}