//
// Copyright (c) Clemens Cords (mail@clemens-cords.com), created 3/23/23
//

#pragma once

#include <include/widget.hpp>

namespace mousetrap
{
    class Spinner : public WidgetImplementation<GtkSpinner>
    {
        public:
            Spinner();

            void set_is_spinning(bool);
            bool get_is_spinning() const;

            void start();
            void stop();
    };
}
