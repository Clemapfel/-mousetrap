//
// Copyright (c) Clemens Cords (mail@clemens-cords.com), created 3/15/23
//

#pragma once

#include <include/widget.hpp>
#include <include/orientation.hpp>

#include <deque>

namespace mousetrap
{
    class Box : public Widget
    {
        public:
            Box(Orientation);

            void push_front(Widget*);
            void push_back(Widget*);
            void remove(Widget*);

            Vector2f get_centroid() const override;
            void set_centroid(Vector2f) override;

            void set_size(Vector2f) override;
            Vector2f get_size() const override;

        private:
            void reformat() override;

            Vector2f _centroid = {0, 0};
            Vector2f _size = {0, 0};

            Orientation _orientation;
    };
}
