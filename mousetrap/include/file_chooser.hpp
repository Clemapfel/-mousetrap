//
// Copyright (c) Clemens Cords (mail@clemens-cords.com), created 3/24/23
//

#pragma once

#include <include/widget.hpp>
#include <include/window.hpp>
#include <include/signal_component.hpp>

namespace mousetrap
{
    /// @brief type of action for file chooser dialog, determines layout and choice type
    enum class FileChooserAction
    {
        /// @brief open file, can accept with one or more files selected
        OPEN = GTK_FILE_CHOOSER_ACTION_OPEN,

        /// @brief save to a path, adds path entry
        SAVE = GTK_FILE_CHOOSER_ACTION_SAVE,

        /// @brief select folder, similar to file but only folders are shown
        SELECT_FOLDER = GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER
    };

    /// @brief native file chooser dialog
    class FileChooser : public WidgetImplementation<GtkFileChooserNative>,
        HAS_SIGNAL(FileChooser, response)
    {
        public:
            /// @brief construct
            /// @param action type of action, determines formatting of the file chooser
            /// @param title title of the window, may be empty
            /// @param accept_label label for the button that confirms the users choice
            /// @param cancel_label label for the button that aborts the users choice
            FileChooser(FileChooserAction, const std::string& title, const std::string& accept_label = "Accept", const std::string& cancel_label = "Cancel");

            /// @brief set label for button that confirms users choice
            /// @param label
            void set_accept_label(const std::string&);

            /// @brief get label for button that confirms users choice
            /// @return label
            std::string get_accept_label() const;

            /// @brief set label for button that aborts dialog
            /// @param label
            void set_cancel_label(const std::string&);

            /// @brief get label for button that aborts dialog
            /// @return label
            std::string get_cancel_label() const;

            /// @brief present the dialog to the user
            void show();

            /// @brief hide the dialog, this does not cause the "response" signal to be emitted
            void hide();

            /// @brief get whether dialog is currently presented to the user
            bool get_is_shown() const;

            /// @brief set whether the dialogs window is modal
            /// @param b true if it should be modal, false otherwise
            void set_is_modal(bool);

            /// @brief get whether the dialogs window is modal
            /// @return true if modal, false otherwise
            bool get_is_modal() const;

            /// @brief set whether dialog should be transient for another window. If transient, dialog will always be shown on top
            void set_transient_for(Window*);
    };
}