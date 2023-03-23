//
// Copyright (c) Clemens Cords (mail@clemens-cords.com), created 3/23/23
//

#pragma once

#include <include/widget.hpp>
#include <include/orientation.hpp>

namespace mousetrap
{
    class Separator : public WidgetImplementation<GtkSeparator>
    {
        public:
            Separator(Orientation);
    };
}
