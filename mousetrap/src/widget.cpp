//
// Created by clem on 3/15/23.
//

#include <include/widget.hpp>

namespace mousetrap
{
    Rectangle Widget::get_bounds() const
    {
        auto size = get_size();
        auto center = get_centroid();
        return Rectangle{{center.x - 0.5 * size.x, center.y - 0.5 * size.y}, size};
    }

    Rectangle Widget::get_bounds_with_margin() const
    {
        auto size = get_size();
        Vector2f top_left = {get_centroid().x - 0.5 * size.x, get_centroid().y - 0.5 * size.y};

        auto margin = get_margin();
        size.x += margin.right;
        size.y += margin.bottom;
        top_left.x -= margin.left;
        top_left.y -= margin.top;

        return Rectangle{top_left, size};
    }
}