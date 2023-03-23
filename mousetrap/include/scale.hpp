//
// Copyright 2022 Clemens Cords
// Created on 7/31/22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <gtk/gtk.h>

#include <include/widget.hpp>
#include <include/adjustment.hpp>
#include <include/orientation.hpp>
#include <include/relative_position.hpp>

namespace mousetrap
{
    class Scale : public WidgetImplementation<GtkScale>,
        HAS_SIGNAL(Scale, value_changed)
    {
        public:
            Scale(float min, float max, float step, Orientation orientation = Orientation::HORIZONTAL);

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

            void set_should_draw_value(bool);
            bool get_should_draw_value() const;

            void set_has_origin(bool);
            bool get_has_origin() const;

            void add_mark(float at, RelativePosition pos, const std::string& label = "");
            void clear_marks();

            template<typename Function_t, typename Data_t>
            void set_format_value_function(Function_t function, Data_t);

            template<typename Function_t>
            void set_format_value_function(Function_t function);

            void reset_format_value_function();

        private:
            Adjustment _adjustment;

            static char* on_format_value(GtkScale* scale, double value, Scale* instance);
            std::function<std::string(float)> _format_f;
    };
}

#include <src/scale.inl>

namespace mousetrap
{
    template<typename Function_t, typename Data_t>
    void Scale::set_format_value_function(Function_t function, Data_t data)
    {
        _format_f = [f = function, d = data](float value) -> std::string{
            return f(value, d);
        };

        gtk_scale_set_format_value_func(get_native(), (GtkScaleFormatValueFunc) on_format_value, this, (GDestroyNotify) nullptr);
    }

    template<typename Function_t>
    void Scale::set_format_value_function(Function_t function)
    {
        _format_f = [f = function](float value) -> std::string{
            return f(value);
        };

        gtk_scale_set_format_value_func(get_native(), (GtkScaleFormatValueFunc) on_format_value, this, (GDestroyNotify) nullptr);
    }
}



