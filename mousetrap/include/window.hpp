//
// Copyright 2022 Clemens Cords
// Created on 8/5/22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <include/widget.hpp>

namespace mousetrap
{
    class Application;

    class Window : public WidgetImplementation<GtkWindow>,
        HAS_SIGNAL(Window, close_request),
        HAS_SIGNAL(Window, activate_default_widget),
        HAS_SIGNAL(Window, activate_focused_widget)
    {
        public:
            Window(Application&);

            void set_maximized(bool);
            void set_fullscreen(bool);

            void present();
            void set_show_menubar(bool);
            void close();

            void set_child(Widget*);
            void remove_child();

            void set_focused_widget(Widget*);
            void set_hide_on_close(bool);
            void set_destroy_with_parent(bool);

            void set_title(const std::string&);

            // https://docs.gtk.org/gtk4/property.Settings.gtk-decoration-layout.html
            void set_titlebar_layout(const char*);
            void set_titlebar_widget(Widget*);

            void set_modal(bool);
            void set_transient_for(Window* partner);
            void set_decorated(bool);
    };
}

