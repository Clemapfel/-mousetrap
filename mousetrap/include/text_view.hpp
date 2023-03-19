//
// Copyright (c) Clemens Cords (mail@clemens-cords.com), created 3/19/23
//

#pragma once

#include <include/widget.hpp>

namespace mousetrap
{
    class TextView : public WidgetImplementation<GtkTextView>
    {
        public:
            TextView();
    };
}
