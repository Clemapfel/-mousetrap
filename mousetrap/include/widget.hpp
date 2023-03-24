//
// Copyright 2022 Clemens Cords
// Created on 7/31/22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <gtk/gtk.h>

#include <include/vector.hpp>
#include <include/signal_emitter.hpp>
#include <include/signal_component.hpp>
#include <include/cursor_type.hpp>
#include <include/alignment.hpp>

/*
#include <include/event_controller.hpp>
#include <include/gtk_common.hpp>
*/

#include <map>
#include <string>
#include <exception>

namespace mousetrap
{
    template<typename GtkWidget_t>
    struct WidgetImplementation;
    struct EventController;

    class Widget : public SignalEmitter,
        HAS_SIGNAL(Widget, realize),
        HAS_SIGNAL(Widget, unrealize),
        HAS_SIGNAL(Widget, destroy),
        HAS_SIGNAL(Widget, hide),
        HAS_SIGNAL(Widget, show),
        HAS_SIGNAL(Widget, map),
        HAS_SIGNAL(Widget, unmap)
    {
        template<typename T>
        friend struct WidgetImplementation;

        public:
            Widget(const Widget&) = delete;
            Widget(Widget&&) noexcept;

            Widget& operator=(const Widget&) = delete;
            Widget& operator=(Widget&&) noexcept;

            virtual operator GtkWidget*() const; // can be overridden if template argument is not parent widget type of compound widget
            operator GObject*() override;

            Vector2f get_size_request();
            void set_size_request(Vector2f);
            Vector2f get_size();

            bool operator==(const Widget& other) const;
            bool operator!=(const Widget& other) const;

            void set_margin_top(float);
            void set_margin_bottom(float);
            void set_margin_start(float);
            void set_margin_end(float);
            void set_margin(float);
            void set_margin_horizontal(float);
            void set_margin_vertical(float);

            void set_expand_horizontally(bool should_expand);
            void set_expand_vertically(bool should_expand);
            void set_expand(bool both);

            void set_horizontal_alignment(Alignment);
            void set_vertical_alignment(Alignment);
            void set_alignment(Alignment both);

            void set_opacity(float);
            float get_opacity();

            void set_visible(bool);
            bool get_visible();

            void set_tooltip_text(const std::string&);
            void set_tooltip_widget(Widget*);

            // for pictures, c.f. https://docs.gtk.org/gdk4/ctor.Cursor.new_from_name.html
            void set_cursor(CursorType type);

            void hide();
            void show();
            void add_controller(EventController*);

            void set_focusable(bool);
            void grab_focus();
            void set_focus_on_click(bool);
            bool get_has_focus();

            bool get_is_realized();

            using preferred_size = struct {Vector2f minimum_size; Vector2f natural_size;};
            preferred_size get_preferred_size();

            void unparent();
            void set_can_respond_to_input(bool);
            bool get_can_respond_to_input() const;

            void beep();

            /// \brief f(FrameClock, Arg_t data) -> Bool
            /// \returns true if tick should continue, false if it should break
            template<typename Function_t, typename Arg_t>
            void add_tick_callback(Function_t, Arg_t);

        protected:
            Widget() = delete;
            void override_native(GtkWidget*);

        private:
            Widget(GtkWidget*);

            virtual ~Widget();

            GtkWidget* _native;

            std::function<bool(GdkFrameClock*)> _tick_callback_f;
            std::function<void(void*)> _destroy_notify_f;

            static gboolean tick_callback_wrapper(GtkWidget*, GdkFrameClock*, Widget* instance);

            Widget* _tooltip_widget = nullptr;
            static gboolean on_query_tooltip(GtkWidget*, gint x, gint y, gboolean, GtkTooltip* tooltip, Widget* instance);
    };

    template<typename GtkWidget_t>
    class WidgetImplementation : public Widget
    {
        public:
            operator GtkWidget_t*() const;

        protected:
            WidgetImplementation(GtkWidget_t*);
            GtkWidget_t* get_native() const;
    };
};

#include <src/widget.inl>


