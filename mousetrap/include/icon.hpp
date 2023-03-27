//
// Copyright (c) Clemens Cords (mail@clemens-cords.com), created 3/27/23
//

#pragma once

#include <gtk/gtk.h>
#include <string>

#include <include/window.hpp>

namespace mousetrap
{
    using IconID = std::string;

    class IconTheme;

    class Icon
    {
        public:
            Icon();

            operator GIcon*() const;
            operator GtkIconPaintable*() const;

            void create_from_file(const std::string& path, size_t square_resolution, size_t scale = 1);
            void create_from_theme(const IconTheme& theme, const IconID&, size_t square_resolution, size_t scale = 1);

            IconID get_name() const;

            bool operator==(const Icon& other) const;
            bool operator!=(const Icon& other) const;

            size_t get_scale() const;
            Vector2ui get_size() const;

        private:
            GIcon* _native = nullptr;
            GtkIconPaintable* _paintable = nullptr;

            size_t _resolution = 0;
            size_t _scale = 1;
    };

    class IconTheme
    {
        public:
            IconTheme();
            IconTheme(const Window&);

            operator GtkIconTheme*() const;

            [[nodiscard]] std::vector<std::string> get_icon_names() const;

            bool has_icon(const Icon&) const;
            bool has_icon(const IconID&) const;

            void add_resource_path(const std::string&);

        private:
            GtkIconTheme* _native;
    };
}
