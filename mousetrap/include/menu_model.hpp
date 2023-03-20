//
// Copyright 2022 Clemens Cords
// Created on 8/29/22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <gtk/gtk.h>
#include <include/widget.hpp>
#include <include/action.hpp>

#include <string>
#include <set>

namespace mousetrap
{
    class MenuModel
    {
        friend class PopoverMenu;

        public:
            MenuModel();
            ~MenuModel();

            void add_action(
                const std::string& label,
                const Action& action,
                bool use_markup = true
            );

            void add_stateful_action(
                const std::string& label,
                const Action& action,
                bool initial_state,
                bool use_markup = true
            );

            void add_widget(Widget*);

            enum SectionFormat
            {
                NORMAL,
                HORIZONTAL_BUTTONS,
                HORIZONTAL_BUTTONS_LEFT_TO_RIGHT,
                HORIZONTAL_BUTTONS_RIGHT_TO_LEFT,
                CIRCULAR_BUTTONS,
                INLINE_BUTTONS
            };

            void add_section(
                const std::string& label,
                MenuModel*,
                SectionFormat = NORMAL
            );

            void add_submenu(const std::string& label, MenuModel*);

            operator GMenuModel*();

        protected:
            std::unordered_map<std::string, Widget*> get_widgets();

        private:
            GMenu* _native;

            static inline size_t current_id = 1;
            std::unordered_map<std::string, Widget*> _id_to_widget;
            std::set<MenuModel*> _submenus;
    };
}
