//
// Copyright 2022 Clemens Cords
// Created on 6/27/22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <include/gl_common.hpp>

#include <vector>
#include <unordered_set>

namespace mousetrap
{
    struct Triangle
    {
        Vector2f a, b, c;
    };

    struct Rectangle
    {
        Vector2f top_left;
        Vector2f size;

        Vector2f get_top_left() const;
        void set_top_left(Vector2f);

        Vector2f get_center_top() const;
        void set_top_center(Vector2f);

        Vector2f get_top_right() const;
        void set_top_right(Vector2f);

        Vector2f get_center_left() const;
        void set_center_left(Vector2f);

        Vector2f get_center() const;
        void set_center(Vector2f);

        Vector2f get_center_right() const;
        void set_center_right(Vector2f);

        Vector2f get_bottom_left() const;
        void set_bottom_left(Vector2f);

        Vector2f get_center_bottom() const;
        void set_center_bottom(Vector2f);

        Vector2f get_bottom_right() const;
        void set_bottom_right(Vector2f);
    };

    struct Circle
    {
        Vector2f center;
        float radius;
    };

    struct Line
    {
        Vector2f a, b;
    };

    bool is_point_in_rectangle(Vector2f point, Rectangle rectangle);

    bool intersecting(Line first, Line second, Vector2f* intersect = nullptr);
    bool intersecting(Line line, Rectangle rectangle, std::vector<Vector2f>* intersections = nullptr);
}
