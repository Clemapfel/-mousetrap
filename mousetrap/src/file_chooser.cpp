
//
// Created by clem on 3/31/23.
//

#include <include/file_chooser.hpp>

namespace mousetrap
{
    FileChooser::FileChooser(FileChooserAction action, const std::string& title, const std::string& accept_label, const std::string& cancel_label)
        : WidgetImplementation<GtkFileChooserNative>(gtk_file_chooser_native_new(title.c_str(), nullptr, (GtkFileChooserAction) action, accept_label.c_str(), cancel_label.c_str())),
          CTOR_SIGNAL(FileChooser, response)
    {}

    void FileChooser::set_accept_label(const std::string& label)
    {
        gtk_file_chooser_native_set_accept_label(get_native(), label.c_str());
    }

    std::string FileChooser::get_accept_label() const
    {
        auto* label = gtk_file_chooser_native_get_accept_label(get_native());
        return label == nullptr ? "" : label;
    }

    void FileChooser::set_cancel_label(const std::string& label)
    {
        gtk_file_chooser_native_set_cancel_label(get_native(), label.c_str());
    }

    std::string FileChooser::get_cancel_label() const
    {
        auto* label = gtk_file_chooser_native_get_cancel_label(get_native());
        return label == nullptr ? "" : label;
    }

    void FileChooser::show()
    {
        gtk_native_dialog_show(GTK_NATIVE_DIALOG(get_native()));
    }

    void FileChooser::hide()
    {
        gtk_native_dialog_hide(GTK_NATIVE_DIALOG(get_native()));
    }

    bool FileChooser::get_is_shown() const
    {
        gtk_native_dialog_get_visible(GTK_NATIVE_DIALOG(get_native()));
    }

    void FileChooser::set_is_modal(bool b)
    {
        gtk_native_dialog_set_modal(GTK_NATIVE_DIALOG(get_native()), b);
    }

    bool FileChooser::get_is_modal() const
    {
        return gtk_native_dialog_get_modal(GTK_NATIVE_DIALOG(get_native()));
    }

    void FileChooser::set_transient_for(Window* window)
    {
        gtk_native_dialog_set_transient_for(GTK_NATIVE_DIALOG(get_native()), window != nullptr ? window->operator GtkWindow*() : nullptr);
    }
}