//
// Copyright 2022 Clemens Cords
// Created on 8/26/22 by clem (mail@clemens-cords.com)
//

#include <include/application.hpp>
#include <include/log.hpp>
#include <iostream>

namespace mousetrap
{
    Application::Application(const std::string& id)
        : CTOR_SIGNAL(Application, activate),
          CTOR_SIGNAL(Application, shutdown)
    {
        log::initialize();

        if (not g_application_id_is_valid(id.c_str()))
            log::critical("In Application::Application: id " + id + " is not a valid application id", MOUSETRAP_DOMAIN);

        _native = gtk_application_new(id.c_str(), G_APPLICATION_DEFAULT_FLAGS);
        _native = g_object_ref(_native);
    }

    Application::~Application()
    {
        g_object_unref(_native);

        for (auto& pair : _actions)
            g_object_unref(pair.second);
    }

    ApplicationID Application::get_id() const
    {
        auto* id = g_application_get_application_id(G_APPLICATION(_native));
        if (id != nullptr)
            return std::string(id);
        else
            return "";
    }

    int Application::run()
    {
        return g_application_run(G_APPLICATION(_native), 0, nullptr);
    }

    void Application::quit()
    {
        g_application_quit(G_APPLICATION(_native));
    }

    void Application::hold()
    {
        if (not _holding)
            g_application_hold(G_APPLICATION(_native));
    }

    void Application::release()
    {
        if (not _holding)
            log::warning("In Application::release: Attempting to release application, but it is not currently holding");
        else
            g_application_release(G_APPLICATION(_native));
    }

    void Application::mark_as_busy()
    {
        if (not _busy)
            g_application_mark_busy(G_APPLICATION(_native));
    }

    void Application::unmark_as_busy()
    {
        if (not _busy)
            log::warning("In Application::unmark_as_busy: Application is not currently marked busy");
        else
            g_application_unmark_busy(G_APPLICATION(_native));
    }

    Application::operator GObject*()
    {
        return G_OBJECT(_native);
    }

    Application::operator GApplication*()
    {
        return G_APPLICATION(_native);
    }

    Application::operator GtkApplication*()
    {
        return GTK_APPLICATION(_native);
    }

    Application::operator GActionMap*()
    {
        return G_ACTION_MAP(_native);
    }

    void Application::add_action(const Action& action)
    {
        if (action == nullptr)
            return;

        if (action.operator GAction *() == nullptr)
            log::warning("In Application::add_action: Attempting to add action `" + action.get_id() + "` to application, but the actions behavior was not set yet. Call Action::set_function or Action::set_stateful_function first");


        auto inserted = _actions.insert({action.get_id(), (detail::ActionInternal*) action.operator GObject*()}).first->second;
        g_action_map_add_action(G_ACTION_MAP(_native), G_ACTION(inserted->g_action));

        auto* app = operator GtkApplication*();

        auto accels = std::vector<const char*>();
        for (auto& s : inserted->shortcuts)
        {
            if (s != "never")
                accels.push_back(s.c_str());
        }
        accels.push_back(NULL);

        if (not accels.empty())
            gtk_application_set_accels_for_action(app, ("app." + inserted->id).c_str(), accels.data());
    }

    void Application::remove_action(const ActionID& id)
    {
        if (not has_action(id))
            log::warning("In Application::remove_action: No action with id `" + id + "` registered");

        auto* self = operator GActionMap*();
        _actions.erase(id);
        g_action_map_remove_action(self, ("app." + id).c_str());
    }

    bool Application::has_action(const ActionID& id)
    {
        return _actions.find(id) != _actions.end();
    }

    Action Application::get_action(const ActionID& id)
    {
        auto it = _actions.find(id);
        if (it == _actions.end())
        {
            log::critical("In Application::get_action: No action with id `" + id + "` registered");
            return Action(nullptr);
        }
        else
            return Action(it->second);
    }

    void Application::set_menubar(MenuModel* model)
    {
        gtk_application_set_menubar(_native, model->operator GMenuModel*());
    }
}
