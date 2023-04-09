//
// Copyright (c) Clemens Cords (mail@clemens-cords.com), created 3/20/23
//

#include <iostream>

namespace mousetrap
{
    namespace detail
    {
        using ActionFunction = std::function<void()>;
    }

    template<typename Function_t>
    void Action::set_function(Function_t function)
    {
        _internal->stateless_f = [f = std::function<void()>(function)](){
            f();
        };

        _internal->stateful_f = nullptr;
        _internal->g_action = (g_simple_action_new(_internal->id.c_str(), nullptr));
        g_signal_connect(G_OBJECT(_internal->g_action), "activate", G_CALLBACK(on_action_activate), _internal);
        set_enabled(_internal->enabled);
    }

    template<typename Function_t, typename Data_t>
    void Action::set_function(Function_t function, Data_t data)
    {
        _internal->stateless_f = [f = std::function<void(Data_t)>(function), d = data](){
            f(d);
        };

        _internal->stateful_f = nullptr;
        _internal->g_action = (g_simple_action_new(_internal->id.c_str(), nullptr));
        g_signal_connect(G_OBJECT(_internal->g_action), "activate", G_CALLBACK(on_action_activate), _internal);
        set_enabled(_internal->enabled);
    }

    template<typename Function_t>
    void Action::set_stateful_function(Function_t function, bool initial_state)
    {
        _internal->stateful_f = [this, f = std::function<bool(bool)>(function)]() -> void
        {
            auto result = f(g_variant_get_boolean(g_action_get_state(G_ACTION(_internal->g_action))));
            g_action_change_state(G_ACTION(_internal->g_action), g_variant_new_boolean(result));
        };

        _internal->stateless_f = nullptr;

        auto* variant = g_variant_new_boolean(true);
        _internal->g_action = (g_simple_action_new_stateful(_internal->id.c_str(), G_VARIANT_TYPE_BOOLEAN, variant));
        g_signal_connect(G_OBJECT(_internal->g_action), "activate", G_CALLBACK(on_action_activate), _internal);
        set_enabled(_internal->enabled);
    }

    template<typename Function_t, typename Data_t>
    void Action::set_stateful_function(Function_t function, Data_t data, bool initial_state)
    {
        _internal->stateful_f = [this, f = std::function<bool(bool)>(function), d = data]() -> void
        {
            auto result = f(g_variant_get_boolean(g_action_get_state(G_ACTION(_internal->g_action))), d);
            g_action_change_state(G_ACTION(_internal->g_action), g_variant_new_boolean(result));
        };

        _internal->stateless_f = nullptr;

        auto* variant = g_variant_new_boolean(true);
        _internal->g_action = (g_simple_action_new_stateful(_internal->id.c_str(), G_VARIANT_TYPE_BOOLEAN, variant));
        g_signal_connect(G_OBJECT(_internal->g_action), "activate", G_CALLBACK(on_action_activate), _internal);
        set_enabled(_internal->enabled);
    }
}
