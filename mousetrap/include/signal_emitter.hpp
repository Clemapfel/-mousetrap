//
// Copyright 2022 Clemens Cords
// Created on 8/26/22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <gtk/gtk.h>

#include <string>
#include <map>
#include <vector>

namespace mousetrap
{
    class SignalEmitter
    {
        public:
            void set_signal_blocked(const std::string& signal_id, bool);
            std::vector<std::string> get_all_signal_names();

            template<typename Function_t>
            void connect_signal(const std::string& signal_id, Function_t*, void* data = nullptr);
            void disconnect_signal(const std::string& signal_id);

            virtual operator GObject*() = 0;

        private:
            struct SignalHandler
            {
                size_t id;
                bool is_blocked = false;
            };

            std::map<std::string, SignalHandler> _signal_handlers = {};
    };
}

#include <src/signal_emitter.inl>