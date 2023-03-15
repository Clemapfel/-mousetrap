//
// Created by clem on 3/15/23.
//

#include <include/overlay.hpp>
#include <iostream>

namespace mousetrap
{
    Overlay::Overlay()
    {}

    void Overlay::add(Widget* widget)
    {
        append_child(widget);
    }

    void Overlay::remove(Widget* widget)
    {
        remove_child(widget);
    }

    Vector2f Overlay::get_centroid() const
    {
        return _centroid;
    }

    void Overlay::set_centroid(Vector2f centroid)
    {
        _centroid = centroid;
        reformat();
    }

    void Overlay::set_size(Vector2f size)
    {
        _size = size;
        reformat();
    }

    Vector2f Overlay::get_size() const
    {
        return _size;
    }

    void Overlay::reformat()
    {

    }
}