//
// Copyright 2022 Clemens Cords
// Created on 8/26/22 by clem (mail@clemens-cords.com)
//

#include <include/image_display.hpp>
#include <include/log.hpp>

#include <iostream>

namespace mousetrap
{
    ImageDisplay::ImageDisplay()
        : WidgetImplementation<GtkImage>(GTK_IMAGE(gtk_image_new()))
    {
        _size = {0, 0};
    }

    ImageDisplay::ImageDisplay(GtkImage* image)
        : WidgetImplementation<GtkImage>(image)
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

    ImageDisplay::ImageDisplay(const Icon& icon)
        : ImageDisplay()
    {
        create_from_icon(icon);
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
            log::critical("In ImageDisplay::create_from_file: Unable to open file at \"" + path + "\": " + std::string(error->message), MOUSETRAP_DOMAIN);
            g_error_free(error);
        }

        gtk_image_set_from_pixbuf(get_native(), pixbuf);
        g_object_unref(pixbuf);
    }

    void ImageDisplay::create_from_icon(const Icon& icon)
    {
        _size = icon.get_size() * Vector2ui(icon.get_scale());
        gtk_image_set_from_paintable(get_native(), GDK_PAINTABLE(icon.operator GtkIconPaintable*()));
    }

    void ImageDisplay::create_as_file_preview(const FileDescriptor& file)
    {
        GError* error = nullptr;
        auto* pixbuf_maybe = gdk_pixbuf_new_from_file(file.get_path().c_str(), &error);
        g_error_free(error);

        if (pixbuf_maybe != nullptr)
        {
            gtk_image_set_from_pixbuf(get_native(), pixbuf_maybe);
            g_object_unref(pixbuf_maybe);
        }
        else
        {
            auto* icon = g_content_type_get_icon(file.query_info(G_FILE_ATTRIBUTE_STANDARD_CONTENT_TYPE).c_str());
            gtk_image_set_from_gicon(get_native(), G_ICON(icon));
            g_object_unref(icon);
        }
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