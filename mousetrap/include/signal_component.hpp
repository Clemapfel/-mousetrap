//
// Copyright (c) Clemens Cords (mail@clemens-cords.com), created 3/17/23
//

#pragma once

#include <functional>

namespace mousetrap
{
    #define HAS_SIGNAL(T, signal_name) \
        public has_##signal_name##_signal<T>

    #define DECLARE_SIGNAL(signal_name, g_signal_id)                             \
        template<typename Owner_t>                                                                    \
        class has_##signal_name##_signal                                                            \
        {                                                                                             \
            private:                                                                                  \
                Owner_t* _instance = nullptr;                                                         \
                std::function<void(Owner_t*)> _function;                                              \
                bool _blocked = false;                                                                \
                                                                                                      \
                static void wrapper(void*, has_##signal_name##_signal<Owner_t>* instance)           \
                {                                                                                     \
                    instance->emit_signal_activate();                                                 \
                }                                                                                     \
                                                                                                      \
            protected:                                                                                \
                explicit has_##signal_name##_signal(Owner_t* instance)                              \
                    : _instance(instance)                                                             \
                {}                                                                                    \
                                                                                                      \
            public:                                                                                   \
                static inline constexpr const char* signal_id = #g_signal_id;                         \
                                                                                                      \
                template<typename Function_t, typename Data_t>                                        \
                void connect_signal_##signal_name(Function_t function, Data_t data)               \
                {                                                                                     \
                    _function = [f = function, d = data](Owner_t* instance)                           \
                    {                                                                                 \
                        f(instance, d);                                                               \
                    };                                                                                \
                                                                                                      \
                    static_cast<SignalEmitter*>(_instance)->connect_signal(signal_id, wrapper, this); \
                }                                                                                     \
                                                                                                      \
                template<typename Function_t>                                                         \
                void connect_signal_##signal_name(Function_t function)                            \
                {                                                                                     \
                    _function = [f = function](Owner_t* instance)                                     \
                    {                                                                                 \
                        f(instance);                                                                  \
                    };                                                                                \
                    static_cast<SignalEmitter*>(_instance)->connect_signal(signal_id, wrapper, this); \
                }                                                                                     \
                                                                                                      \
                void set_signal_##signal_name##_blocked(bool b)                                   \
                {                                                                                     \
                    _blocked = b;                                                                     \
                }                                                                                     \
                                                                                                      \
                bool get_signal_##signal_name##_blocked() const                                   \
                {                                                                                     \
                    return _blocked;                                                                  \
                }                                                                                     \
                                                                                                      \
                void emit_signal_activate()                                                           \
                {                                                                                     \
                    if (not _blocked)                                                                 \
                        _function(_instance);                                                         \
                }                                                                                     \
        };
    
    DECLARE_SIGNAL(activate, activate);
    DECLARE_SIGNAL(startup, startup)
    DECLARE_SIGNAL(shutdown, shutdown)
}
