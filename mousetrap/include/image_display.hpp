//
// Copyright 2022 Clemens Cords
// Created on 8/26/22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <include/image.hpp>
#include <include/widget.hpp>

namespace mousetrap
{
    /// @brief widget that display an iamge
    class ImageDisplay : public WidgetImplementation<GtkImage>
    {
        public:
            /// @brief default ctor, initialize as 0x0 image
            ImageDisplay();

            /// @brief construct from image on disk
            /// @param path
            ImageDisplay(const std::string& path);

            /// @brief construct from image
            /// @param image
            ImageDisplay(const Image&);

            /// @brief get resolution of image buffer
            /// @return resolution
            Vector2ui get_size() const;

            /// @brief load from image on disk
            /// @param path
            void create_from_file(const std::string& path);

            /// @brief load from image
            /// @param image
            void create_from_image(const Image&);

            /// @brief create from empty image
            void clear();

            /// @brief set scale, will use linear interpolation. For other types of interpolation, scale a mousetrap::Image instead
            /// @param pixel_size: scale (positive integer) or -1 for default size
            void set_scale(int);

        private:
            Vector2ui _size;
    };
}
