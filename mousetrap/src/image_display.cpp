//
// Copyright 2022 Clemens Cords
// Created on 8/26/22 by clem (mail@clemens-cords.com)
//

#include <include/image_display.hpp>
#include <iostream>

namespace mousetrap
{
    ImageDisplay::ImageDisplay()
        : WidgetImplementation<GtkImage>(GTK_IMAGE(gtk_image_new()))
    {
        _size = {0, 0};
    }

    ImageDisplay::ImageDisplay(const std::string& file)
       : ImageDisplay()
    {
        create_from_file(file);
    }

    ImageDisplay::ImageDisplay(const Image& image)
        : ImageDisplay()
    {
        create_from_image(image);
    }

    Vector2ui ImageDisplay::get_size() const
    {
        return _size;
    }

    void ImageDisplay::create_from_image(const Image& image)
    {
        gtk_image_clear(get_native());
        gtk_image_set_from_pixbuf(get_native(), image.operator GdkPixbuf*());
    }

    void ImageDisplay::create_from_file(const std::string& path)
    {
        gtk_image_clear(get_native());

        GError* error = nullptr;
        auto* pixbuf = gdk_pixbuf_new_from_file(path.c_str(), &error);

        if (error != nullptr)
        {
            std::cerr << "[WARNING] In ImageDisplay::create_from_file: Unable to open file at \"" << path << "\": " << error->message << std::endl;
            g_error_free(error);
        }

        gtk_image_set_from_pixbuf(get_native(), pixbuf);
        g_object_unref(pixbuf);
    }

    void ImageDisplay::clear()
    {
        gtk_image_clear(get_native());
    }

    void ImageDisplay::set_scale(int scale)
    {
        if (scale < -1)
            scale = -1;

        gtk_image_set_pixel_size(get_native(), scale);
    }
}