//
// Copyright (c) Clemens Cords (mail@clemens-cords.com), created 3/20/23
//

#pragma once

#include <gtk/gtk.h>

namespace mousetrap
{
    enum class PopoverPosition
    {
        ABOVE = GTK_POS_TOP,
        LEFT_OF = GTK_POS_LEFT,
        RIGHT_OF = GTK_POS_RIGHT,
        BELOW = GTK_POS_BOTTOM
    };
}
