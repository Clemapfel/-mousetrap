//
// Copyright (c) Clemens Cords (mail@clemens-cords.com), created 3/23/23
//

#pragma once

#include <include/widget.hpp>

namespace mousetrap
{
    class ProgressBar : public WidgetImplementation<GtkProgressBar>
    {
        public:
            ProgressBar();

            void pulse();

            /// @param value: float in [0, 1]
            void set_fraction(float);
            float get_fraction() const;

            bool get_is_inverted() const;
            void set_is_inverted(bool);

            void set_text(const std::string&);
            std::string get_text() const;

            void set_show_text_or_percentage(bool);
            bool get_show_text_or_percentage() const;

            void set_progress_inverted(bool);
            bool get_progress_inverted() const;
    };
}
