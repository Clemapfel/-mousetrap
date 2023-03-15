//
// Copyright (c) Clemens Cords (mail@clemens-cords.com), created 3/15/23
//

#pragma once

#include <include/alignable.hpp>
#include <include/resizable.hpp>
#include <include/movable.hpp>
#include <include/geometry.hpp>

#include <deque>

namespace mousetrap
{
    class Widget : public Alignable, public Movable, public Resizable
    {
        public:
            /*
            Widget(const Widget&) = delete;
            Widget(Widget&&) = default;
            Widget& operator=(const Widget&) = delete;
            Widget& operator=(Widget&&) = delete;
             */

            Vector2f get_centroid() const = 0;
            void set_centroid(Vector2f) = 0;

            void set_size(Vector2f) = 0;
            Vector2f get_size() const = 0;

            void set_reformatting_blocked(bool);

        protected:
            Widget();

            Widget* get_parent() const;
            void set_parent(Widget*);

            void append_child(Widget*);
            void prepend_child(Widget*);
            void remove_child(Widget*);
            bool has_child(Widget*);
            const std::deque<Widget*>& get_children() const;

            virtual void reformat() = 0;

        private:
            Widget* _parent = nullptr;

            std::deque<Widget*> _children;

            bool _reformatting_blocked = false;
    };
}
