//
// Created by clem on 3/27/23.
//

#include <include/icon.hpp>
#include <iostream>

namespace mousetrap
{
    Icon::Icon()
        : _native(nullptr)
    {}

    Icon::operator GIcon*() const
    {
        return _native;
    }

    Icon::operator GtkIconPaintable*() const
    {
        return _paintable;
    }

    size_t Icon::get_scale() const
    {
        return _scale;
    }

    Vector2ui Icon::get_size() const
    {
        return {_resolution, _resolution};
    }

    void Icon::create_from_file(const std::string& path, size_t square_resolution, size_t scale)
    {
        if (_scale == 0)
            _scale = 1;

        auto* file = g_file_new_for_path(path.c_str());
        _native = g_file_icon_new(file);
        _paintable = gtk_icon_paintable_new_for_file(file, square_resolution, scale);

        g_free(file);
        _resolution = square_resolution;
        _scale = scale;
    }

    void Icon::create_from_theme(const IconTheme& theme, const IconID& id, size_t square_resolution, size_t scale)
    {
        _resolution = square_resolution;
        _scale = scale;

        GError* error = nullptr;
        _native = g_icon_new_for_string(id.c_str(), &error);

        _paintable = gtk_icon_theme_lookup_icon(
            theme.operator GtkIconTheme*(),
            id.c_str(),
            nullptr,
            square_resolution,
            scale,
            GtkTextDirection::GTK_TEXT_DIR_LTR,
            GtkIconLookupFlags::GTK_ICON_LOOKUP_FORCE_REGULAR
        );

        if (error != nullptr)
        {
            std::cerr << "[ERROR] In Icon::create_from_theme: " << error->message << std::endl;
            g_error_free(error);
        }
    }

    IconID Icon::get_name() const
    {
        if (_native == nullptr)
            return IconID();

        return g_icon_to_string(_native);
    }

    bool Icon::operator==(const Icon& other) const
    {
        if (this->_native == nullptr or other._native == nullptr)
            return false;

        return g_icon_equal(this->_native, other._native);
    }

    bool Icon::operator!=(const Icon& other) const
    {
        if (this->_native == nullptr or other._native == nullptr)
            return false;

        return not g_icon_equal(this->_native, other._native);
    }

    IconTheme::IconTheme()
        : _native(gtk_icon_theme_new())
    {}

    IconTheme::IconTheme(const Window& window)
        : _native(gtk_icon_theme_get_for_display(gtk_widget_get_display(window.operator GtkWidget*())))
    {}

    IconTheme::operator GtkIconTheme*() const
    {
        return _native;
    }

    std::vector<IconID> IconTheme::get_icon_names() const
    {
        std::vector<std::string> out;
        char** list = gtk_icon_theme_get_icon_names(_native);
        size_t i = 0;
        while (list[i] != nullptr)
        {
            out.push_back(list[i]);
            i += 1;
        }

        g_strfreev(list);
        return out;
    }

    bool IconTheme::has_icon(const Icon& icon) const
    {
        return gtk_icon_theme_has_gicon(_native, icon.operator GIcon*());
    }

    bool IconTheme::has_icon(const IconID& id) const
    {
        return gtk_icon_theme_has_icon(_native, id.c_str());
    }

    void IconTheme::add_resource_path(const std::string& path)
    {
        gtk_icon_theme_add_resource_path(_native, path.c_str());
        gtk_icon_theme_add_search_path(_native, path.c_str());
    }
}
