//
// Copyright (c) Clemens Cords (mail@clemens-cords.com), created 3/17/23
//

#include <include/frame_clock.hpp>
#include <iostream>

namespace mousetrap
{
    template<typename T>
    WidgetImplementation<T>::WidgetImplementation(T* in)
        : Widget(GTK_WIDGET(in))
    {
        if (not GTK_IS_WIDGET(in))
            throw std::invalid_argument("[FATAL] In WidgetImplementation::WidgetImplementation(T*): Object is not a widget.");

        g_signal_connect(in, "destroy", G_CALLBACK(on_destroy), this);
    }

    template<typename T>
    WidgetImplementation<T>::operator T*() const
    {
        return (T*) Widget::operator GtkWidget*();
    }

    template<typename T>
    T* WidgetImplementation<T>::get_native() const
    {
        return (T*) Widget::operator GtkWidget*();
    }

    template<typename Function_t, typename Data_t>
    void Widget::add_tick_callback(Function_t f_in, Data_t data_in)
    {
        _tick_callback_f = [f = f_in, data = data_in](GdkFrameClock* clock) -> bool {
            return (bool) f(FrameClock(clock), data);
        };

        gtk_widget_add_tick_callback(_native, (GtkTickCallback) G_CALLBACK(tick_callback_wrapper), this, nullptr);
    }

    template<typename Function_t>
    void Widget::add_tick_callback(Function_t f_in)
    {
        _tick_callback_f = [f = f_in](GdkFrameClock* clock) -> bool {
            return (bool) f(FrameClock(clock));
        };

        gtk_widget_add_tick_callback(_native, (GtkTickCallback) G_CALLBACK(tick_callback_wrapper), this, nullptr);
    }
}