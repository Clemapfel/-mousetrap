//
// Copyright (c) Clemens Cords (mail@clemens-cords.com), created 3/15/23
//

#pragma once

#include <include/vector.hpp>
#include <limits>

namespace mousetrap
{
    class Resizable
    {
        public:
            using SizeRequest = struct
            {
                float min_width = 0;
                float max_width = std::numeric_limits<float>::max();
                float min_height = 0;
                float max_height = std::numeric_limits<float>::max();
            };

            void set_minimum_size_request(Vector2f);
            void set_maximum_size_request(Vector2f);
            SizeRequest get_size_request() const;

            virtual void set_size(Vector2f) = 0;
            virtual Vector2f get_size() const = 0;

            void set_should_expand_horizontally(bool);
            bool get_should_expand_horizontally() const;

            void set_should_expand_vertically(bool);
            bool get_should_expand_vertically() const;

            using Margin = struct
            {
                float left = 0;
                float right = 0;
                float top = 0;
                float bottom = 0;
            };

            void set_margin_left(float);
            void set_margin_right(float);
            void set_margin_top(float);
            void set_margin_bottom(float);
            void set_margin_horizontal(float);
            void set_margin_vertical(float);
            void set_margin(float);

            Margin get_margin() const;

        private:
            SizeRequest _size_request;
            Vector2f _size;

            Margin _margin;

            bool _should_expand_horizontally = true;
            bool _should_expand_vertically = true;
    };
}