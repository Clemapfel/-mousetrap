//
// Copyright (c) Clemens Cords (mail@clemens-cords.com), created 3/17/23
//

#pragma once

namespace mousetrap
{
    enum class CursorType
    {
        NONE,
        DEFAULT,
        HELP,
        POINTER,
        CONTEXT_MENU,
        PROGRESS,
        WAIT,
        CELL,
        CROSSHAIR,
        TEXT,
        MOVE,
        NOT_ALLOWED,
        GRAB,
        GRABBING,
        ALL_SCROLL,
        ZOOM_IN,
        ZOOM_OUT,
        COLUMN_RESIZE,
        ROW_RESIZE,
        NORTH_RESIZE,
        NORTH_EAST_RESIZE,
        EAST_RESIZE,
        SOUTH_EAST_RESIZE,
        SOUTH_RESIZE,
        SOUTH_WEST_RESIZE,
        WEST_RESIZE,
        NORTH_WEST_RESIZE,
        HORIZONTAL_RESIZE = COLUMN_RESIZE,
        VERTICAL_RESIZE = ROW_RESIZE,
    };

}