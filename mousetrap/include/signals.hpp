//
// Copyright (c) Clemens Cords (mail@clemens-cords.com), created 3/15/23
//

#pragma once

#include <functional>

namespace mousetrap
{
    #define DECLARE_SIGNAL(CamelCaseName, snake_case_name)                 \
    class Has##CamelCaseName##Signal                                       \
    {                                                                      \
        public:                                                            \
            void emit_signal_##snake_case_name()                           \
            {                                                              \
                if (not _blocked)                                          \
                    _f();                                                  \
            }                                                              \
                                                                           \
            template<typename Function_t, typename Data_t>                 \
            void connect_signal_##snake_case_name(Function_t f_in, Data_t data_in)      \
            {                                                              \
                _f = [f = f_in, data = data_in]() -> void {                \
                    f(data);                                               \
                };                                                         \
            };                                                             \
                                                                           \
            void disconnect_signal_##snake_case_name()                     \
            {                                                              \
                _f = nullptr;                                              \
            }                                                              \
                                                                           \
            void set_signal_##snake_case_name##_blocked(bool b)            \
            {                                                              \
                _blocked = b;                                              \
            }                                                              \
                                                                           \
            bool get_signal_##snake_case_name##_blocked() const            \
            {                                                              \
                return _blocked;                                           \
            }                                                              \
                                                                           \
        private:                                                           \
            bool _blocked = false;                                         \
            std::function<void()> _f;                                      \
    };                                                                     \

    DECLARE_SIGNAL(Woof, woof);
}