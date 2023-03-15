//
// Copyright (c) Clemens Cords (mail@clemens-cords.com), created 3/15/23
//

#pragma once

#include <include/widget.hpp>
#include <include/shape.hpp>
#include <include/color.hpp>

namespace mousetrap
{
    class RectangleShape : public Widget, public Shape
    {
        public:
            RectangleShape();
            RectangleShape(Vector2f top_left, Vector2f size);

            void set_color(RGBA);
            RGBA get_color() const;

            Vector2f get_centroid() const override;
            void set_centroid(Vector2f) override;

            void set_size(Vector2f) override;
            Vector2f get_size() const override;

            const Shape* get_shape() const;

        protected:
            void reformat() override {};

        private:
            Shape _gl_shape;
            RGBA _color = RGBA(1, 1, 1, 1);
    };
}