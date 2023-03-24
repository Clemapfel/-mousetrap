//
// Copyright (c) Clemens Cords (mail@clemens-cords.com), created 3/20/23
//

#pragma once

#include <include/widget.hpp>
#include <include/orientable.hpp>
#include <include/adjustment.hpp>

namespace mousetrap
{
    class Scrollbar : public WidgetImplementation<GtkScrollbar>, public Orientable
    {
        public:
            Scrollbar(Orientation);
            ~Scrollbar();

            Adjustment& get_adjustment();
            const Adjustment& get_adjustment() const;

            void set_orientation(Orientation) override;
            Orientation get_orientation() const override;

        private:
            Adjustment* _adjustment = nullptr;
    };
}
