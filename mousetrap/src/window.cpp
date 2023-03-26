//
// Copyright 2022 Clemens Cords
// Created on 8/15/22 by clem (mail@clemens-cords.com)
//

#include <include/window.hpp>
#include <include/application.hpp>

namespace mousetrap
{
    Window::Window(Application& app)
        : WidgetImplementation<GtkWindow>(GTK_WINDOW(gtk_application_window_new(app.operator GtkApplication*()))),
          CTOR_SIGNAL(Window, close_request),
          CTOR_SIGNAL(Window, activate_default_widget),
          CTOR_SIGNAL(Window, activate_focused_widget)
    {}

    void Window::present()
    {
        gtk_window_present(get_native());
    }

    void Window::close()
    {
        gtk_window_close(get_native());
    }

    void Window::set_show_menubar(bool b)
    {
        gtk_application_window_set_show_menubar(GTK_APPLICATION_WINDOW(get_native()), b);
    }

    void Window::set_maximized(bool b)
    {
        if (b)
            gtk_window_maximize(get_native());
        else
            gtk_window_unmaximize(get_native());
    }

    void Window::set_fullscreen(bool b)
    {
        if (b)
            gtk_window_fullscreen(get_native());
        else
            gtk_window_unfullscreen(get_native());
    }

    void Window::set_child(Widget* widget)
    {
        _child = widget;
        gtk_window_set_child(get_native(), _child == nullptr ? nullptr : _child->operator GtkWidget*());
    }

    Widget* Window::get_child() const
    {
        return _child;
    }


    void Window::set_hide_on_close(bool b)
    {
        gtk_window_set_hide_on_close(get_native(), b);
    }

    void Window::set_title(const std::string& str)
    {
        gtk_window_set_title(get_native(), str.c_str());
    }

    std::string Window::get_title() const
    {
        return gtk_window_get_title(get_native());
    }

    void Window::set_destroy_with_parent(bool b)
    {
        gtk_window_set_destroy_with_parent(get_native(), b);
    }

    bool Window::get_destroy_with_parent() const
    {
        return gtk_window_get_destroy_with_parent(get_native());
    }

    void Window::set_titlebar_layout(const char* layout)
    {
        auto* titlebar = GTK_HEADER_BAR(gtk_header_bar_new());
        gtk_header_bar_set_decoration_layout(titlebar, layout);
        gtk_window_set_titlebar(get_native(), GTK_WIDGET(titlebar));
    }

    void Window::set_titlebar_widget(Widget* widget)
    {
        _titlebar_widget = widget;
        gtk_window_set_titlebar(get_native(), _titlebar_widget == nullptr ? nullptr : _titlebar_widget->operator GtkWidget*());
    }

    Widget* Window::get_titlebar_widget() const
    {
        return _titlebar_widget;
    }

    void Window::set_is_modal(bool b)
    {
        gtk_window_set_modal(get_native(), b);
    }

    bool Window::get_is_modal() const
    {
        return gtk_window_get_modal(get_native());
    }

    void Window::set_transient_for(Window* partner)
    {
        if (partner != nullptr)
            gtk_window_set_transient_for(get_native(), partner->operator GtkWindow*());
    }

    void Window::set_is_decorated(bool b)
    {
        gtk_window_set_decorated(get_native(), b);
    }

    bool Window::get_is_decorated() const
    {
        return gtk_window_get_decorated(get_native());
    }

    void Window::set_has_close_button(bool b)
    {
        gtk_window_set_deletable(get_native(), b);
    }

    bool Window::get_has_close_button() const
    {
        return gtk_window_get_deletable(get_native());
    }

    void Window::set_startup_notification_identifier(const std::string& str)
    {
        gtk_window_set_startup_id(get_native(), str.c_str());
    }

    void Window::set_focus_visible(bool b)
    {
        gtk_window_set_focus_visible(get_native(), b);
    }

    bool Window::get_focus_visible() const
    {
        return gtk_window_get_focus_visible(get_native());
    }
}