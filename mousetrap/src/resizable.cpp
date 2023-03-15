//
// Created by clem on 3/15/23.
//

#include <include/resizable.hpp>

namespace mousetrap
{
    void Resizable::set_minimum_size_request(Vector2f xy)
    {
        _size_request.min_width = xy.x;
        _size_request.min_height = xy.x;
    }

    void Resizable::set_maximum_size_request(Vector2f xy)
    {
        _size_request.max_height = xy.x;
        _size_request.max_width = xy.x;
    }

    Resizable::SizeRequest Resizable::get_size_request() const
    {
        return _size_request;
    }

    void Resizable::set_should_expand_horizontally(bool b)
    {
        _should_expand_horizontally = b;
    }

    bool Resizable::get_should_expand_horizontally() const
    {
        return _should_expand_horizontally;
    }

    void Resizable::set_should_expand_vertically(bool b)
    {
        _should_expand_vertically = b;
    }

    bool Resizable::get_should_expand_vertically() const
    {
        return _should_expand_vertically;
    }

    void Resizable::set_margin_left(float margin)
    {
        _margin.left = margin;
    }

    void Resizable::set_margin_right(float margin)
    {
        _margin.right = margin;
    }

    void Resizable::set_margin_top(float margin)
    {
        _margin.top = margin;
    }

    void Resizable::set_margin_bottom(float margin)
    {
        _margin.bottom = margin;
    }

    void Resizable::set_margin_horizontal(float margin)
    {
        set_margin_left(margin);
        set_margin_right(margin);
    }

    void Resizable::set_margin_vertical(float margin)
    {
        set_margin_top(margin);
        set_margin_bottom(margin);
    }

    void Resizable::set_margin(float margin)
    {
        set_margin_left(margin);
        set_margin_right(margin);
        set_margin_top(margin);
        set_margin_bottom(margin);
    }

    Resizable::Margin Resizable::get_margin() const
    {
        return _margin;
    }
}
