//
// Copyright (c) Clemens Cords (mail@clemens-cords.com), created 3/3/23
//

#pragma once

#include <gtk/gtk.h>
#include <string>

#include "image.hpp"
#include "widget.hpp"

namespace mousetrap
{
    class Clipboard
    {
        public:
            Clipboard(Widget*);
            operator GdkClipboard*();

            bool contains_string() const;
            bool contains_image() const;

            bool is_local() const;

            void set_string(const std::string&);

            /// @param Function_t: void(Clipboard*, const std::string&, Data_t)
            template<typename Function_t, typename Data_t>
            bool get_string(Function_t on_string_read, Data_t);

            void set_image(const Image&);

            /// @param Function_t: void(Clipboard*, const Image&, Data_t)
            template<typename Function_t, typename Data_t>
            bool get_image(Function_t on_image_red, Data_t);

        protected:
            Clipboard(GdkClipboard*);

        private:
            GdkClipboard* _native;

            std::function<void(Clipboard*, const std::string&)> get_string_f;
            static void get_string_callback_wrapper(GObject* clipboard, GAsyncResult* result, gpointer data);

            std::function<void(Clipboard*, const Image&)> get_image_f;
            static void get_image_callback_wrapper(GObject* clipboard, GAsyncResult* result, gpointer data);
    };
}

#include <src/clipboard.inl>

