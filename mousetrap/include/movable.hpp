//
// Copyright (c) Clemens Cords (mail@clemens-cords.com), created 3/15/23
//

#pragma once

#include <include/vector.hpp>

namespace mousetrap
{
    class Movable
    {
        public:
            virtual Vector2f get_centroid() const = 0;
            virtual void set_centroid(Vector2f) = 0;
    };
}
