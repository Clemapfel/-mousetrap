//
// Copyright 2022 Clemens Cords
// Created on 8/26/22 by clem (mail@clemens-cords.com)
//

#include <include/application.hpp>

namespace mousetrap
{
    Application::Application()
        : HasActivateSignal<Application>(this)
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

    /*
    void Application::set_menubar(MenuModel* model)
    {
        gtk_application_set_menubar(_native, model->operator GMenuModel*());
    }

    */
}
