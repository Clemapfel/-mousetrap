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

        for (auto* child : children)
        {
            child->set_size(_size);
            child->set_centroid(_centroid);

            std::cout << child->get_size().x << " " << child->get_size().y << std::endl;
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
