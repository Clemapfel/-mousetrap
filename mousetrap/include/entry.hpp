//
// Copyright 2022 Clemens Cords
// Created on 7/31/22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <include/widget.hpp>

namespace mousetrap
{
    class Entry : public WidgetImplementation<GtkEntry>,
        HAS_SIGNAL(Entry, activate),
        HAS_SIGNAL(Entry, text_changed),
        HAS_SIGNAL(Entry, text_inserted),
        HAS_SIGNAL(Entry, text_deleted)
    {
        public:
            Entry();

            std::string get_text() const;
            void set_text(const std::string&);

            void set_max_length(size_t);
            size_t get_max_length() const;

            void set_has_frame(bool);
            bool get_has_frame() const;

            /// @brief enter password mode
            void set_text_visible(bool);
            bool get_text_visible() const;
    };
}
