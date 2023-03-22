//
// Copyright 2022 Clemens Cords
// Created on 7/31/22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <include/widget.hpp>
#include <include/adjustment.hpp>

namespace mousetrap
{
    class SpinButton : public WidgetImplementation<GtkSpinButton>,
        HAS_SIGNAL(SpinButton, value_changed),
        HAS_SIGNAL(SpinButton, wrapped)
    {
        public:
            SpinButton(float min, float max, float step);

            Adjustment& get_adjustment();
            const Adjustment& get_adjustment() const;

            float get_lower() const;
            float get_upper() const;
            float get_value() const;
            float get_increment() const;

            void set_lower(float);
            void set_upper(float);
            void set_value(float);
            void set_increment(float);

            void set_n_digits(size_t);
            size_t get_n_digits() const;

            void set_should_wrap(bool);
            bool get_should_wrap() const;

            void set_acceleration_rate(float);
            float get_acceleration_rate() const;

            void set_should_snap_to_ticks(bool);
            bool get_should_snap_to_ticks() const;

            void set_allow_only_numeric(bool);
            bool get_allow_only_numeric() const;

            template<typename Function_t, typename Data_t>
            void set_text_to_value_function(Function_t, Data_t);

            template<typename Function_t>
            void set_text_to_value_function(Function_t);

            void reset_text_to_value_function();

            template<typename Function_t, typename Data_t>
            void set_value_to_text_function(Function_t, Data_t);

            template<typename Function_t>
            void set_value_to_text_function(Function_t);

            void reset_value_to_text_function();

        private:
            Adjustment _adjustment;

            static gint on_input(GtkSpinButton*, double* new_value, SpinButton* instance);
            std::function<std::string(const SpinButton*, float)> _value_to_text_f;

            static bool on_output(GtkSpinButton*, SpinButton* instance);
            std::function<float(const SpinButton*, const std::string&)> _text_to_value_f;
    };
}

namespace mousetrap
{
    template<typename Function_t, typename Data_t>
    void SpinButton::set_text_to_value_function(Function_t f_in, Data_t data_in)
    {
        _text_to_value_f = [f = f_in, data = data_in](const SpinButton* instance, const std::string& text) -> float
        {
            return f(instance, text, data);
        };

        connect_signal("input", on_input, this);
        set_allow_only_numeric(false);
    }

    template<typename Function_t>
    void SpinButton::set_text_to_value_function(Function_t f_in)
    {
        _text_to_value_f = [f = f_in](const SpinButton* instance, const std::string& text) -> float
        {
            return f(instance, text);
        };

        connect_signal("input", on_input, this);
        set_allow_only_numeric(false);
    }

    template<typename Function_t, typename Data_t>
    void SpinButton::set_value_to_text_function(Function_t f_in, Data_t data_in)
    {
        _value_to_text_f = [f = f_in, data = data_in](const SpinButton* instance, float value)
        {
            return f(instance, value, data);
        };

        connect_signal("output", on_output, this);
        set_allow_only_numeric(false);
    }

    template<typename Function_t>
    void SpinButton::set_value_to_text_function(Function_t f_in)
    {
        _value_to_text_f = [f = f_in](const SpinButton* instance, float value)
        {
            return f(instance, value);
        };

        connect_signal("output", on_output, this);
        set_allow_only_numeric(false);
    }

}
