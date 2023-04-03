//
// Copyright (c) Clemens Cords (mail@clemens-cords.com), created 3/20/23
//

#include <iostream>
#include <include/g_object_attachment.hpp>

namespace mousetrap
{
    namespace detail
    {
        using ActionFunction = std::function<void()>;
    }

    template<typename Function_t>
    void Action::set_function(Function_t function)
    {
        _stateless_f = new detail::ActionFunction([f = std::function<void()>(function)](){
            f();
        });

        _stateful_f = nullptr;
        _g_action = g_object_ref(g_simple_action_new(_id.c_str(), nullptr));
        g_signal_connect(G_OBJECT(_g_action), "activate", G_CALLBACK(on_action_activate), this);
        set_enabled(_enabled);
    }

    template<typename Function_t, typename Data_t>
    void Action::set_function(Function_t function, Data_t data)
    {
        _stateless_f = new detail::ActionFunction([f = std::function<void(Data_t)>(function), d = data](){
            f(d);
        });

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

    template<typename Function_t, typename Data_t>
    void Action::set_stateful_function(Function_t function, Data_t data, bool initial_state)
    {
        _stateful_f = [this, f = std::function<bool(bool)>(function), d = data]() -> void
        {
            auto result = f(g_variant_get_boolean(g_action_get_state(G_ACTION(_g_action))), d);
            g_action_change_state(G_ACTION(_g_action), g_variant_new_boolean(result));
        };

        _stateless_f = nullptr;

        auto* variant = g_variant_new_boolean(true);
        _g_action = g_object_ref(g_simple_action_new_stateful(_id.c_str(), G_VARIANT_TYPE_BOOLEAN, variant));
        g_signal_connect(G_OBJECT(_g_action), "activate", G_CALLBACK(on_action_activate), this);
        set_enabled(_enabled);
    }
}
