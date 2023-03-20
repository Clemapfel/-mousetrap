//
// Copyright 2022 Clemens Cords
// Created on 9/13/22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <include/widget.hpp>
#include <include/transition_type.hpp>

namespace mousetrap
{
    class Revealer : public WidgetImplementation<GtkRevealer>
    {
        public:
            Revealer(RevealerTransitionType = RevealerTransitionType::CROSSFADE);

            void set_child(Widget*);
            Widget* get_child() const;

            void set_revealed(bool b);
            bool get_revealed() const;

            void set_transition_type(RevealerTransitionType);
            RevealerTransitionType get_transition_type() const;

            void set_transition_duration(Time);
            Time get_transition_duration() const;

        private:
            Widget* _child = nullptr;
    };
}