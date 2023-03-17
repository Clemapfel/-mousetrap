//
// Copyright 2022 Clemens Cords
// Created on 8/15/22 by clem (mail@clemens-cords.com)
//

#include <include/window.hpp>
#include <include/application.hpp>

namespace mousetrap
{
    /*
    Window::Window()
        : Window(GTK_WINDOW(gtk_window_new()))
    {
        gtk_window_set_hide_on_close(get_native(), true);
    }

    Window::Window(GtkWindow* window)
        : WidgetImplementation<GtkWindow>(window)
    {}
     */

    Window::Window(Application* app)
        : WidgetImplementation<GtkWindow>(GTK_WINDOW(gtk_application_window_new(app->operator GtkApplication*())))
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
        gtk_window_set_child(get_native(), widget == nullptr ? nullptr : widget->operator GtkWidget*());
    }

    void Window::remove_child()
    {
        gtk_window_set_child(get_native(), nullptr);
    }

    void Window::set_focused_widget(Widget* widget)
    {
        gtk_window_set_focus(get_native(), widget->operator GtkWidget*());
    }

    void Window::set_hide_on_close(bool b)
    {
        gtk_window_set_hide_on_close(get_native(), b);
    }

    void Window::set_title(const std::string& str)
    {
        gtk_window_set_title(get_native(), str.c_str());
    }

    void Window::set_destroy_with_parent(bool b)
    {
        gtk_window_set_destroy_with_parent(get_native(), b);
    }

    void Window::set_titlebar_layout(const char* layout)
    {
        auto* titlebar = GTK_HEADER_BAR(gtk_header_bar_new());
        gtk_header_bar_set_decoration_layout(titlebar, layout);
        gtk_window_set_titlebar(get_native(), GTK_WIDGET(titlebar));
    }

    void Window::set_titlebar_widget(Widget* widget)
    {
        gtk_window_set_titlebar(get_native(), widget == nullptr ? nullptr : widget->operator GtkWidget*());
    }

    void Window::set_modal(bool b)
    {
        gtk_window_set_modal(get_native(), b);
    }

    void Window::set_transient_for(Window* partner)
    {
        gtk_window_set_transient_for(get_native(), partner->operator GtkWindow*());
    }

    void Window::set_decorated(bool b)
    {
        gtk_window_set_decorated(get_native(), b);
    }
}