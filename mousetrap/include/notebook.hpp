//
// Copyright (c) Clemens Cords (mail@clemens-cords.com), created 3/20/23
//

#pragma once

#include <include/widget.hpp>
#include <include/relative_position.hpp>
#include <include/selection_model.hpp>

namespace mousetrap
{
    class Notebook : public WidgetImplementation<GtkNotebook>,
        HAS_SIGNAL(Notebook, page_added),
        HAS_SIGNAL(Notebook, page_reordered),
        HAS_SIGNAL(Notebook, page_removed),
        HAS_SIGNAL(Notebook, page_selection_changed)
    {
        public:
            Notebook();

            size_t push_front(Widget* child_widget, Widget* label_widget);
            size_t push_back(Widget* child_widget, Widget* label_widget);
            size_t insert(size_t new_position, Widget* child_widget, Widget* label_widget);
            void remove(size_t position);

            void next_page();
            void goto_page(size_t);
            void previous_page();

            /// @returns -1 if no pages
            int32_t get_current_page() const;
            size_t get_n_pages() const;

            void set_is_scrollable(bool);
            bool get_is_scrollable() const;

            void set_has_border(bool);
            bool get_has_border() const;

            void set_tabs_visible(bool);
            bool get_tabs_visible() const;

            void set_quick_change_menu_enabled(bool);
            bool get_quick_change_menu_enabled() const;

            void set_tab_position(RelativePosition);
            RelativePosition get_tab_position() const;

            void set_tabs_reorderable(bool b);
            bool get_tabs_reorderable() const;

        private:
            bool _popup_enabled = false;
            bool _tabs_reorderable = false;
    };
}