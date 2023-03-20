//
// Copyright 2022 Clemens Cords
// Created on 8/26/22 by clem (mail@clemens-cords.com)
//

#include <include/application.hpp>

namespace mousetrap
{
    Application::Application()
        : CTOR_SIGNAL(Application, activate),
          CTOR_SIGNAL(Application, startup),
          CTOR_SIGNAL(Application, shutdown)
    {
        _native = gtk_application_new(nullptr, G_APPLICATION_DEFAULT_FLAGS);
        _native = g_object_ref(_native);
    }

    Application::~Application()
    {
        g_object_unref(_native);
    }

    int Application::run()
    {
        return g_application_run(G_APPLICATION(_native), 0, nullptr);
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

    void Application::add_action(Action* action)
    {
        auto inserted = _actions.insert({action->get_id(), action}).first->second;
        auto* self = operator GActionMap*();
        g_action_map_add_action(self, inserted->operator GAction *());

        auto* app = operator GtkApplication*();

        auto accels = std::vector<const char*>();
        for (auto& s : inserted->get_shortcuts())
        {
            if (s != "never")
                accels.push_back(s.c_str());
        }
        accels.push_back(NULL);

        if (not accels.empty())
            gtk_application_set_accels_for_action(app, ("app." + inserted->get_id()).c_str(), accels.data());
    }

    void Application::remove_action(const ActionID& id)
    {
        auto* self = operator GActionMap*();
        _actions.erase(id);
        g_action_map_remove_action(self, ("app." + id).c_str());
    }

    bool Application::has_action(const ActionID& id)
    {
        return _actions.find(id) != _actions.end();
    }

    Action* Application::get_action(const ActionID& id)
    {
        return _actions.at(id);
    }

    void Application::set_menubar(MenuModel* model)
    {
        gtk_application_set_menubar(_native, model->operator GMenuModel*());
    }
}
