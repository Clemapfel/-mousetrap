//
// Created by clem on 3/15/23.
//

#include <include/box.hpp>
#include <iostream>

namespace mousetrap
{
    Box::Box(Orientation orientation)
        : _orientation(orientation)
    {}

    void Box::push_front(Widget* widget)
    {
        append_child(widget);
    }

    void Box::push_back(Widget* widget)
    {
        prepend_child(widget);
    }

    void Box::remove(Widget* widget)
    {
        remove_child(widget);
    }

    void Box::set_centroid(Vector2f centroid)
    {
        _centroid = centroid;
        reformat();
    }

    Vector2f Box::get_centroid() const
    {
        return _centroid;
    }

    void Box::set_size(Vector2f size)
    {
        _size = size;
        reformat();
    }

    Vector2f Box::get_size() const
    {
        return _size;
    }

    void Box::reformat()
    {
        const auto& children = get_children();

        float min_x_sum = 0;
        float min_y_sum = 0;

        for (auto* child : children)
        {
            auto size = child->get_size();
            auto margin = child->get_margin();

            min_x_sum += size.x + margin.left + margin.right;
            min_y_sum += size.y + margin.top + margin.bottom;
        }

        float left = _centroid.x - 0.5 * _size.x;
        float right = _centroid.x + 0.5 * _size.x;
        float x_space = (right - left) - min_x_sum / children.size();

        for (auto* child : children)
        {
            auto size = child->get_size();
            auto margin = child->get_margin();
            left += x_space;
            child->set_reformatting_blocked(true);
            child->set_size(size);
            child->set_centroid({left + x_space + margin.left + 0.5 * size.x, _centroid.y});
            child->set_reformatting_blocked(false);
        }

        /*
            auto size = get_size();
            auto center = get_centroid();
            return Rectangle{{center.x - 0.5 * size.x, center.y - 0.5 * size.y}, size};

            auto size = get_size();
            Vector2f top_left = {get_centroid().x - 0.5 * size.x, get_centroid().y - 0.5 * size.y};

            auto margin = get_margin();
            size.x += margin.right;
            size.y += margin.bottom;
            top_left.x -= margin.left;
            top_left.y -= margin.top;

            return Rectangle{top_left, size};
        */

    }
}
