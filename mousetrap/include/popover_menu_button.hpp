//
// Copyright 2022 Clemens Cords
// Created on 8/29/22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <include/widget.hpp>
#include <include/menu_model.hpp>
#include <include/relative_position.hpp>
#include <include/popover.hpp>

namespace mousetrap
{
    class PopoverMenu : public WidgetImplementation<GtkPopoverMenu>
    {
        friend class PopoverMenuButton;

        public:
            PopoverMenu(MenuModel*);

        protected:
            void refresh_widgets();

        private:
            MenuModel* _model;
    };

    class PopoverMenuButton : public WidgetImplementation<GtkMenuButton>,
        HAS_SIGNAL(PopoverMenuButton, activate)
    {
        public:
            PopoverMenuButton();

            void set_child(Widget*);
            Widget* get_child() const;

            void set_popover_position(RelativePosition);
            RelativePosition get_popover_position() const;

            void set_popover(Popover*);
            void set_popover_menu(PopoverMenu*);

            void set_always_show_arrow(bool);
            bool get_always_show_arrow() const;

            void set_has_frame(bool);
            bool get_has_frame() const;

            void popup();
            void popdown();

        private:
            Widget* _child = nullptr;
            PopoverMenu* _popover_menu = nullptr;
    };
}
