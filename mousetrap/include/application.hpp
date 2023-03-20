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
    class Application : public SignalEmitter,
        HAS_SIGNAL(Application, activate),
        HAS_SIGNAL(Application, startup),
        HAS_SIGNAL(Application, shutdown)
    {
        public:
            Application();
            virtual ~Application();

            int run();
            operator GObject*() override;
            operator GApplication*();
            operator GtkApplication*();
            operator GActionMap*();

            void add_action(Action*);
            void remove_action(const ActionID&);
            Action* get_action(const ActionID&);
            bool has_action(const ActionID&);

            /// @brief should be done during signal startup
            void set_menubar(MenuModel*);

        private:
            GtkApplication* _native;
            std::unordered_map<ActionID, Action*> _actions;
    };
}
