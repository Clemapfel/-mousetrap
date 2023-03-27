//
// Copyright 2022 Clemens Cords
// Created on 8/25/22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <gtk/gtk.h>

#include <include/signal_emitter.hpp>
#include <include/window.hpp>
#include <include/action.hpp>

#include <include/menu_model.hpp>

#include <string>

namespace mousetrap
{
    /// @brief object representing an entire application, supplies the main render loop, mapping of actions
    class Application : public SignalEmitter,
        HAS_SIGNAL(Application, activate),
        HAS_SIGNAL(Application, startup),
        HAS_SIGNAL(Application, shutdown)
    {
        public:
            /// @brief construct new action
            /// @param id valid application id, see https://docs.gtk.org/gio/type_func.Application.id_is_valid.html
            Application(const std::string& id);

            /// @brief destroy action, should only be called at the very end of <tt>main</tt>
            virtual ~Application();

            /// @brief start the main render loop
            /// @returns 1 on error, 0 otherwise
            int run();

            /// @brief expose as GObject for signal handling, for internal use only
            explicit operator GObject*() override;

            /// @brief expose as GApplication, for internal use only
            explicit operator GApplication*();

            /// @brief expose as GtkApplication, for internal use only
            explicit operator GtkApplication*();

            /// @brief expose as GActionMap, for internal use only
            explicit operator GActionMap*();

            /// @brief add an action to application, see the manual page on actions for details
            /// @param action pointer to action, the user is responsible for making sure the action stays in scope
            void add_action(Action*);

            /// @brief remove an action from application
            /// @param id
            void remove_action(const ActionID&);

            /// @brief lookup action based on action id
            /// @param id
            /// @return pointer to mousetrap::Action if an action with the given id is registered, nullptr otherwise
            Action* get_action(const ActionID&);

            /// @brief check if application has an action with given id registered
            /// @param id
            /// @return true if action with id is registered, false otherwise
            bool has_action(const ActionID&);

            /// @brief set the model used for the applications menubar, this should be called during emission of the <tt>startup<//t> signal
            /// @param model
            void set_menubar(MenuModel*);

        private:
            GtkApplication* _native;
            std::unordered_map<ActionID, Action*> _actions;
    };
}
