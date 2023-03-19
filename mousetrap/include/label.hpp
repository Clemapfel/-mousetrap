//
// Copyright 2022 Clemens Cords
// Created on 8/1/22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <include/widget.hpp>
#include <include/justify_mode.hpp>

namespace mousetrap
{
    enum class EllipsizeMode
    {
        NONE = PANGO_ELLIPSIZE_NONE,
        START = PANGO_ELLIPSIZE_START,
        MIDDLE = PANGO_ELLIPSIZE_MIDDLE,
        END = PANGO_ELLIPSIZE_END,
    };

    enum class LabelWrapMode
    {
        NONE = -1,
        ONLY_ON_WORD = PANGO_WRAP_WORD,
        ONLY_ON_CHAR = PANGO_WRAP_CHAR,
        WORD_OR_CHAR = PANGO_WRAP_WORD_CHAR
    };

    class Label : public WidgetImplementation<GtkLabel>
    {
        public:
            Label();
            Label(const std::string&);

            void set_text(const std::string&);
            std::string get_text();

            void set_use_markup(bool b);
            bool get_use_markup() const;

            void set_ellipsize_mode(EllipsizeMode);
            EllipsizeMode get_ellipsize_mode() const;

            void set_wrap_mode(LabelWrapMode);
            LabelWrapMode get_wrap_mode() const;

            void set_justify_mode(JustifyMode);
            JustifyMode get_justify_mode() const;

            void set_max_width_chars(size_t n_chars);
            size_t get_max_width_chars() const;

            void set_x_alignment(float);
            float get_x_alignment() const;

            void set_y_alignment(float);
            float get_y_alignment() const;

            void set_selectable(bool);
            bool get_selectable() const;
    };
}
