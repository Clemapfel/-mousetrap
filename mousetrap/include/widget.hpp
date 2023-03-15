//
// Copyright (c) Clemens Cords (mail@clemens-cords.com), created 3/15/23
//

#pragma once

#include <include/alignable.hpp>
#include <include/resizable.hpp>
#include <include/movable.hpp>

#include <include/shape.hpp>

namespace mousetrap
{
    class Widget : public Alignable, public Movable, public Resizable
    {
        public:
            Vector2f get_centroid() const = 0;
            void set_centroid(Vector2f) = 0;

            void set_size(Vector2f) = 0;
            Vector2f get_size() const = 0;

            Rectangle get_bounds() const;
            Rectangle get_bounds_with_margin() const;
    };

    class RectangeShape : public Widget
    {
        public:


        private:
            Shape _gl_shape;
    };
}
