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

    class Action
    {
        public:
            /// @param app: Application the action will be registered to, it is only available after `set_function` has been called
            Action(const std::string& id);
            ~Action();

            ActionID get_id() const;

            Action(const Action&) = delete;
            Action(Action&&) noexcept = delete;
            Action& operator=(const Action&) = delete;
            Action& operator=(Action&&) noexcept = delete;

            template<typename Function_t>
            void set_function(Function_t f);

            template<typename Function_t, typename Data_t>
            void set_function(Function_t f, Data_t data);

            template<typename Function_t>
            void set_stateful_function(Function_t f, bool initial_state = false);

            template<typename Function_t, typename Data_t>
            void set_stateful_function(Function_t f, Data_t data, bool initial_state = false);


            void set_state(bool);
            void activate() const;

            void add_shortcut(const ShortcutTriggerID&);
            const std::vector<ShortcutTriggerID>& get_shortcuts() const;

            explicit operator GAction*() const;
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

#include <src/action.inl>
