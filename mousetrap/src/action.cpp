//
// Copyright (c) Clemens Cords (mail@clemens-cords.com), created 1/13/23
//

#include <include/action.hpp>
#include <include/application.hpp>
#include <include/log.hpp>

#include <iostream>

namespace mousetrap
{
    Action::Action(const std::string& id)
        : _id(id)
    {}

    void Action::on_action_activate(GSimpleAction*, GVariant* variant, Action* instance)
    {
        if (instance->_stateless_f)
            instance->_stateless_f();

        if (instance->_stateful_f)
            instance->_stateful_f();
    }

    Action::~Action()
    {
        if (_g_action == nullptr)
            g_object_unref(_g_action);
    }

    void Action::on_action_change_state(GSimpleAction*, GVariant* variant, Action* instance)
    {
        if (instance->_stateless_f)
            instance->_stateless_f();

        if (instance->_stateful_f)
            instance->_stateful_f();
    }

    void Action::activate() const
    {
        if (_stateless_f)
            _stateless_f();
        else if (_stateful_f)
            _stateful_f();

        if (not _stateful_f and not _stateless_f)
            log::warning("In Action::activate: Activating action with id " + get_id() + ", but set_function or set_stateful_function has not been called yet", MOUSETRAP_DOMAIN);
    }

    void Action::set_state(bool b)
    {
        if (get_is_stateful())
            g_action_change_state(G_ACTION(_g_action), g_variant_new_boolean(b));
        else
            log::warning("In Action::set_state: Action with id " + get_id() + " is stateless", MOUSETRAP_DOMAIN);
    }

    bool Action::get_state() const
    {
        if (get_is_stateful())
            return g_variant_get_boolean(g_action_get_state(G_ACTION(_g_action)));
        else
        {
            log::warning("In Action::get_state: Action with id " + get_id() + " is stateless", MOUSETRAP_DOMAIN);
            return false;
        }
    }

    void Action::add_shortcut(const ShortcutTriggerID& shortcut)
    {
        auto* trigger = gtk_shortcut_trigger_parse_string(shortcut.c_str());
        if (trigger == nullptr)
        {
            auto str = std::stringstream();
            str << "In Action::add_shortcut: Unable to parse shortcut trigger `" << shortcut
                      << "`. Ignoring this shortcut binding" << std::endl;
            log::critical(str.str(), MOUSETRAP_DOMAIN);
            return;
        }

        g_object_unref(trigger);
        _shortcuts.push_back(shortcut.c_str());
    }

    const std::vector<ShortcutTriggerID>& Action::get_shortcuts() const
    {
        return _shortcuts;
    }

    Action::operator GAction*() const
    {
        return G_ACTION(_g_action);
    }

    ActionID Action::get_id() const
    {
        return _id;
    }

    void Action::set_enabled(bool b)
    {
        _enabled = b;

        if (_g_action != nullptr)
            g_simple_action_set_enabled(_g_action, b);
    }

    bool Action::get_enabled() const
    {
        return _enabled;
    }

    bool Action::get_is_stateful() const
    {
        return _stateful_f != nullptr;
    }
}