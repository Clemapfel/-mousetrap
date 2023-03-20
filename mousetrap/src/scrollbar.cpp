//
// Copyright 2022 Clemens Cords
// Created on 8/11/22 by clem (mail@clemens-cords.com)
//

#include <include/scrollbar.hpp>

namespace mousetrap
{
    Scrollbar::Scrollbar(Orientation orientation)
        : WidgetImplementation<GtkScrollbar>((GTK_SCROLLBAR(gtk_scrollbar_new((GtkOrientation) orientation, nullptr))))
    {
        _adjustment = new Adjustment(gtk_scrollbar_get_adjustment(get_native()));
    }

    Scrollbar::~Scrollbar()
    {
        delete _adjustment;
    }

    Adjustment& Scrollbar::get_adjustment()
    {
        return *_adjustment;
    }

    const Adjustment& Scrollbar::get_adjustment() const
    {
        return *_adjustment;
    }
}