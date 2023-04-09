//
// Copyright 2022 Clemens Cords
// Created on 7/31/22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <include/widget.hpp>
#include <include/signal_component.hpp>
#include <include/action.hpp>

#ifdef DOXYGEN
    #include "../docs/doxygen.inl"
#endif

namespace mousetrap
{
    #define SIGNAL_INTERNAL_PRIVATE_CLASS_NAME(CamelCase) _Signal##CamelCase##Internal
    #define SIGNAL_INTERNAL_CLASS_NAME(CamelCase) Signal##CamelCase##Internal
    #define SIGNAL_INTERNAL_NEW(snake_case) signal_##snake_case##_internal_new

    #define SIGNAL_CLASS_NAME(snake_case) has_signal_##snake_case

    #define DECLARE_SIGNAL(CamelCase, snake_case, CAPS_CASE, return_t) \
    \
    namespace detail \
    { \
        struct SIGNAL_INTERNAL_PRIVATE_CLASS_NAME(CamelCase) \
        { \
            GObject parent; \
            void* instance; \
            std::function<return_t(void*)> function; \
            bool blocked; \
        }; \
        using SIGNAL_INTERNAL_CLASS_NAME(CamelCase) = SIGNAL_INTERNAL_PRIVATE_CLASS_NAME(CamelCase); \
        SIGNAL_INTERNAL_CLASS_NAME(CamelCase)* SIGNAL_INTERNAL_NEW(snake_case)(void* instance); \
    } \
    \
    template<typename T> \
    class SIGNAL_CLASS_NAME(snake_case) \
    { \
        private: \
            detail::SIGNAL_INTERNAL_CLASS_NAME(Clicked)* _internal;

            static return_t wrapper(void*, detail::SIGNAL_INTERNAL_CLASS_NAME(Clicked)* internal)
            {
                if (not internal->blocked and internal->function)
                    return internal->function(internal->instance);
                else
                    return return_t();
            }

        protected:
            SIGNAL_CLASS_NAME(clicked)(T* instance)
                : _internal(detail::SIGNAL_INTERNAL_NEW(clicked)((void*) instance))
            {}

        public:
            static inline constexpr const char* signal_id = "clicked";

            template<typename Function_t, typename Data_t>
            void connect_signal_clicked(Function_t f, Data_t data)
            {
                _internal->function = [f, data](void* instance) -> return_t {
                    return f((T*) instance, data);
                };
                ((T*) _internal->instance)->connect_signal(signal_id, wrapper, _internal);
            }

            template<typename Function_t>
            void connect_signal_clicked(Function_t f)
            {
                _internal->function = [f](void* instance) -> return_t {
                    return f((T*) instance);
                };
                ((T*) _internal->instance)->connect_signal(signal_id, wrapper, _internal);
            }

            void set_signal_clicked_blocked(bool b)
            {
                _internal->blocked = b;
            }

            bool get_signal_clicked_blocked() const
            {
                return _internal->blocked;
            }

            return_t emit_signal_clicked()
            {
                return wrapper(nullptr, _internal);
            }

            void disconnect_signal_clicked()
            {
                ((T*) _internal->instance)->disconnect_signal(signal_id);
            }
    };
}
// TODO

namespace mousetrap
{
    /// @brief button, triggers action or signal when clicked
    class Button : public WidgetImplementation<GtkButton>,
        HAS_SIGNAL(Button, activate),
        public has_signal_clicked<Button>
        //HAS_SIGNAL(Button, clicked)
    {
        public:
            /// @brief construct
            Button();

            /// @brief set whether button should have a black outline
            /// @param b
            void set_has_frame(bool b);

            /// @brief get whether button has a black outline
            /// @return bool
            bool get_has_frame() const;

            /// @brief set widget used as the buttons label
            /// @param widget
            void set_child(Widget*);

            /// @brief get widget used as the buttons label
            /// @return widget
            Widget* get_child() const;

            /// @brief set action triggered when the button is activated, if the action is disabled the button is also disabled automatically
            void set_action(const Action&);

        private:
            Widget* _child = nullptr;
    };
}
