//
// Copyright (c) Clemens Cords (mail@clemens-cords.com), created 3/19/23
//

#pragma once

#include <gtk/gtk.h>

namespace mousetrap
{
    enum class JustifyMode
    {
        LEFT = GTK_JUSTIFY_LEFT,
        RIGHT = GTK_JUSTIFY_RIGHT,
        CENTER = GTK_JUSTIFY_CENTER,
        FILL = GTK_JUSTIFY_FILL
    };
}