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
    /// @brief object representing the data in the systems clipboard
    class Clipboard
    {
        public:
            /// @brief constructor from widget, usually a window
            Clipboard(Widget*);

            /// @brief expose as GdkClipboard, for internal use only
            operator GdkClipboard*();

            /// @brief check if the clipboard contains a string
            /// @return true if string, false otherwise
            bool contains_string() const;

            /// @brief check if the cliboard contains an image
            /// @return true if image, false otherwise
            bool contains_image() const;

            /// @brief check if the clipboards content was set from within the same application the widget belongs to
            /// @return boolean
            bool is_local() const;

            /// @brief override the data in the clipboard with a string
            /// @param string
            void set_string(const std::string&);

            /// @brief concurrently read the data in the clipboard. Once reading is done, <tt>on_string_read</tt>> will be called with the returned string
            /// @param Function_t lamdba with signature <tt>(Clipboard*, const std::string&, Data_t) -> void</tt>
            /// @param data arbitrary data
            /// @return true if data can be serialized into a string, false otherwise
            template<typename Function_t, typename Data_t>
            bool get_string(Function_t on_string_read, Data_t);

            /// @brief concurrently read the data in the clipboard. Once reading is done, <tt>on_string_read</tt>> will be called with the returned string
            /// @param Function_t lamdba with signature <tt>(Clipboard*, const std::string&) -> void</tt>
            /// @return true if data can be serialized into a string, false otherwise
            template<typename Function_t>
            bool get_string(Function_t on_string_read);

            /// @brief override data in clipboard with an image
            /// @param image
            void set_image(const Image&);

            /// @brief concurrently read the data in the clipboard. Once reading is done, <tt>on_string_read</tt>> will be called with the returned image
            /// @param Function_t lamdba with signature <tt>(Clipboard*, const Image&, Data_t) -> void</tt>
            /// @param data arbitrary data
            /// @return true if data can be serialized into an image, false otherwise
            template<typename Function_t, typename Data_t>
            bool get_image(Function_t on_image_read, Data_t);

            /// @brief concurrently read the data in the clipboard. Once reading is done, <tt>on_string_read</tt>> will be called with the returned image
            /// @param Function_t lamdba with signature <tt>(Clipboard*, const Image&) -> void</tt>
            /// @return true if data can be serialized into an image, false otherwise
            template<typename Function_t>
            bool get_image(Function_t on_image_read);

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

