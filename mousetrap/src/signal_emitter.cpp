//
// Copyright 2022 Clemens Cords
// Created on 8/26/22 by clem (mail@clemens-cords.com)
//

#include <iostream>
#include <include/signal_emitter.hpp>
#include <include/log.hpp>

namespace mousetrap
{
    void SignalEmitter::set_signal_blocked(const std::string& signal_id, bool b)
    {
        auto it = _signal_handlers.find(signal_id);

        if (it == _signal_handlers.end())
        {
            log::warning("In Widget::set_signal_blocked: no signal with id \"" + signal_id + "\" connected.", MOUSETRAP_DOMAIN);
            return;
        }

        if (b)
        {
            if (not it->second.is_blocked)
            {
                g_signal_handler_block(operator GObject*(), it->second.id);
                it->second.is_blocked = true;
            }
        }
        else
        {
            if (it->second.is_blocked)
            {
                g_signal_handler_unblock(operator GObject*(), it->second.id);
                it->second.is_blocked = false;
            }
        }
    }

    void SignalEmitter::disconnect_signal(const std::string& signal_id)
    {
        auto it = _signal_handlers.find(signal_id);
        if (it == _signal_handlers.end())
            return;

        g_signal_handler_disconnect(operator GObject*(), it->second.id);
    }

    static void test()
    {
        std::cout << "test" << std::endl;
    }

    void SignalEmitter::new_signal(const std::string& signal_id)
    {
        auto handler = g_signal_newv(signal_id.c_str(),
                                     G_TYPE_FROM_INSTANCE(operator GObject*()),
                      (GSignalFlags) (G_SIGNAL_RUN_LAST | G_SIGNAL_NO_RECURSE | G_SIGNAL_NO_HOOKS),
                       NULL,
                       NULL,
                       NULL,
                       NULL,
                       G_TYPE_NONE ,
                       0,
                       NULL);

        /*
        g_signal_connect(operator GObject*(), signal_id.c_str(), G_CALLBACK(test), nullptr);
        g_signal_emit(operator GObject*(), handler, 0);
         */
    }

    std::vector<std::string> SignalEmitter::get_all_signal_names()
    {
        std::vector<std::string> out;
        guint n;
        auto* ids = g_signal_list_ids(gtk_file_chooser_get_type(), &n);
        for (size_t i = 0; i < n; ++i)
            out.emplace_back(g_signal_name(ids[i]));

        g_free(ids);
        return out;
    }
}