//
// Created by clem on 3/15/23.
//

#include <include/rectangle_shape.hpp>

namespace mousetrap
{
    RectangleShape::RectangleShape()
    {
        _gl_shape.as_rectangle({0, 0}, {1, 1});
    }

    RectangleShape::RectangleShape(Vector2f top_left, Vector2f size)
    {
        _gl_shape.as_rectangle(top_left, size);
    }

    Vector2f RectangleShape::get_centroid() const
    {
        auto top_left = _gl_shape.get_top_left();
        auto size = _gl_shape.get_size();

        return {top_left.x + 0.5 + size.x, top_left.y + 0.5 * size.y};
    }

    void RectangleShape::set_centroid(Vector2f centroid)
    {
        _gl_shape.set_centroid(centroid);
    }

    void RectangleShape::set_size(Vector2f size)
    {
        auto centroid = _gl_shape.get_centroid();
        _gl_shape.as_rectangle({
        centroid.x - 0.5 * size.x,
        centroid.y - 0.5 * size.y,
        }, size);
    }

    Vector2f RectangleShape::get_size() const
    {
        return _gl_shape.get_size();
    }

    void RectangleShape::set_color(RGBA color)
    {
        _color = color;
        _gl_shape.set_color(color);
    }

    RGBA RectangleShape::get_color() const
    {
        return _color;
    }

    const Shape* RectangleShape::get_shape() const
    {
        return &_gl_shape;
    }
}