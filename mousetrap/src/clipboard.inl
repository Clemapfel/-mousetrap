//
// Copyright (c) Clemens Cords (mail@clemens-cords.com), created 3/20/23
//

namespace mousetrap
{
    template <typename Function_t, typename Data_t>
    bool Clipboard::get_string(Function_t f_in, Data_t data_in)
    {
        get_string_f = [f = f_in, data = data_in](Clipboard* instance, const std::string& str){
            f(instance, str, data);
        };
        gdk_clipboard_read_text_async(_native, nullptr, Clipboard::get_string_callback_wrapper, (gpointer) this);
        return contains_string();
    }

    template <typename Function_t>
    bool Clipboard::get_string(Function_t f_in)
    {
        get_string_f = [f = f_in](Clipboard* instance, const std::string& str){
            f(instance, str);
        };
        gdk_clipboard_read_text_async(_native, nullptr, Clipboard::get_string_callback_wrapper, (gpointer) this);
        return contains_string();
    }

    template <typename Function_t, typename Data_t>
    bool Clipboard::get_image(Function_t f_in, Data_t data_in)
    {
        get_image_f = [f = f_in, data = data_in](Clipboard* instance, const Image& str){
            f(instance, str, data);
        };
        gdk_clipboard_read_texture_async(_native, nullptr, Clipboard::get_image_callback_wrapper, (gpointer) this);
        return contains_image();
    }

    template <typename Function_t>
    bool Clipboard::get_image(Function_t f_in)
    {
        get_image_f = [f = f_in](Clipboard* instance, const Image& str){
            f(instance, str);
        };
        gdk_clipboard_read_texture_async(_native, nullptr, Clipboard::get_image_callback_wrapper, (gpointer) this);
        return contains_image();
    }
}