//
// Copyright (c) Clemens Cords (mail@clemens-cords.com), created 3/19/23
//

#pragma once

#include <include/widget.hpp>
#include <include/justify_mode.hpp>

namespace mousetrap
{
    /// @brief vastly simplified multi-line text entry, obfuscates many of Gtk4s TextViews function for the sake of simplicity
    class TextView : public WidgetImplementation<GtkTextView>,
        HAS_SIGNAL(TextView, text_changed),
        HAS_SIGNAL(TextView, text_inserted),
        HAS_SIGNAL(TextView, text_deleted),
        HAS_SIGNAL(TextView, undo),
        HAS_SIGNAL(TextView, redo)
    {
        public:
            TextView();

            [[nodiscard]] std::string get_text() const;
            void set_text(const std::string& text) const;

            void set_cursor_visible(bool);
            bool get_cursor_visible() const;

            void undo();
            void redo();

            void set_was_modified(bool);
            bool get_was_modified() const;

            void set_editable(bool);
            bool get_editable() const;

            void set_justify_mode(JustifyMode);
            JustifyMode get_justify_mode() const;

            void set_left_margin(float);
            float get_left_margin() const;

            void set_right_margin(float);
            float get_right_margin() const;

            void set_top_margin(float);
            float get_top_margin() const;

            void set_bottom_margin(float);
            float get_bottom_margin() const;
    };
}
