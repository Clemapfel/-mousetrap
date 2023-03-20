//
// Copyright (c) Clemens Cords (mail@clemens-cords.com), created 3/20/23
//

#pragma once

#include <include/widget.hpp>
#include <include/orientation.hpp>
#include <include/adjustment.hpp>

namespace mousetrap
{
    class Scrollbar : public WidgetImplementation<GtkScrollbar>
    {
        public:
            Scrollbar(Orientation);
            ~Scrollbar();

            Adjustment& get_adjustment();
            const Adjustment& get_adjustment() const;


        private:
            Adjustment* _adjustment = nullptr;
    };
}
