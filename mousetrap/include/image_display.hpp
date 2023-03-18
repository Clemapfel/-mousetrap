//
// Copyright 2022 Clemens Cords
// Created on 8/26/22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <include/image.hpp>
#include <include/widget.hpp>

namespace mousetrap
{
    class ImageDisplay : public WidgetImplementation<GtkImage>
    {
        public:
            ImageDisplay();
            ImageDisplay(const std::string& path);
            ImageDisplay(const Image&);

            Vector2ui get_size() const;

            void create_from_file(const std::string& path);
            void create_from_image(const Image&);
            void clear();

            /// @param pixel_size: scale (positive integer) or -1 for default size
            void set_scale(int);

        private:
            Vector2ui _size;
    };
}
