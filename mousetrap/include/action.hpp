//
// Copyright (c) Clemens Cords (mail@clemens-cords.com), created 1/13/23
//

#pragma once

#include <gtk/gtk.h>
#include <string>
#include <functional>
#include <deque>

namespace mousetrap
{
    using ActionID = std::string;
    using ShortcutTriggerID = std::string;

    #define DECLARE_GLOBAL_ACTION(group, key) inline mousetrap::Action group##_##key = Action(#group + std::string(".") + #key);

    // Shortcuts
    // To make an action activatable, the following steps have to be done:
    // 1) Create the action and set a function via `Action::set_function`
    // 2) Add a shortcut via `Action::add_shortcut`
    // 3) Register the action to the application with `Application::add_action`
    // 4) Create a shortcut controller
    // 5) Add the action to the shortcut controller using `ShortcutController::add_action`
    // 6) Add the controller to a widget, for example a window

    class Action
    {
        public:
            Action(const std::string& id);
            ~Action();

            ActionID get_id() const;

            template<typename Function_t>
            void set_function(Function_t f);

            template<typename Function_t>
            void set_stateful_function(Function_t f, bool initial_state = false);

            void set_state(bool);
            void activate() const;

            void add_shortcut(const ShortcutTriggerID&);
            const std::vector<ShortcutTriggerID>& get_shortcuts() const;

            operator GAction*() const;
            void set_enabled(bool);
            bool get_enabled() const;

            bool get_is_stateful() const;

        private:
            ActionID _id;
            std::vector<ShortcutTriggerID> _shortcuts;

            static void on_action_activate(GSimpleAction*, GVariant*, Action*);
            static void on_action_change_state(GSimpleAction*, GVariant*, Action*);

            GSimpleAction* _g_action = nullptr;
            GVariant* _g_state = nullptr;

            std::function<void()> _stateless_f = nullptr;
            std::function<void()> _stateful_f = nullptr;

            bool _enabled = true;
    };
}

namespace mousetrap
{
    template<typename Function_t>
    void Action::set_function(Function_t function)
    {
        _stateless_f = [f = std::function<void()>(function)](){
            f();
        };

        _stateful_f = nullptr;
        _g_action = g_object_ref(g_simple_action_new(_id.c_str(), nullptr));
        g_signal_connect(G_OBJECT(_g_action), "activate", G_CALLBACK(on_action_activate), this);
        set_enabled(_enabled);
    }

    template<typename Function_t>
    void Action::set_stateful_function(Function_t function, bool initial_state)
    {
        _stateful_f = [this, f = std::function<bool(bool)>(function)]() -> void
        {
            auto result = f(g_variant_get_boolean(g_action_get_state(G_ACTION(_g_action))));
            g_action_change_state(G_ACTION(_g_action), g_variant_new_boolean(result));
        };

        _stateless_f = nullptr;

        auto* variant = g_variant_new_boolean(true);
        _g_action = g_object_ref(g_simple_action_new_stateful(_id.c_str(), G_VARIANT_TYPE_BOOLEAN, variant));
        g_signal_connect(G_OBJECT(_g_action), "activate", G_CALLBACK(on_action_activate), this);
        set_enabled(_enabled);
    }
}
