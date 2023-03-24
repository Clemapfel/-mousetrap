//
// Copyright (c) Clemens Cords (mail@clemens-cords.com), created 3/23/23
//

#pragma once

#include <include/widget.hpp>
#include <include/orientable.hpp>

namespace mousetrap
{
    class Separator : public WidgetImplementation<GtkSeparator>, public Orientable
    {
        public:
            Separator(Orientation);

            void set_orientation(Orientation) override;
            Orientation get_orientation() const override;
    };
}
