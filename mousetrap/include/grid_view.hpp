//
// Copyright 2022 Clemens Cords
// Created on 9/22/22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <include/widget.hpp>
#include <include/selection_model.hpp>
#include <include/orientable.hpp>

namespace mousetrap
{
    /// @brief container, arranges widget in a grid
    class GridView : public WidgetImplementation<GtkGridView>, public Orientable,
        HAS_SIGNAL(GridView, activate)
    {
        public:
            /// @brief construct
            /// @param orientation if horizontal, widgets will be arranged left to right, if vertical, widgets will be arranged top to bottom
            /// @param selection_mode
            GridView(Orientation = Orientation::VERTICAL, SelectionMode = SelectionMode::NONE);

            /// @brief add widget to the left if orientation is horizontal, to the top if vertical
            /// @param widget
            void push_front(Widget*);

            /// @brief add widget to the right if orientation is horizontal, to the bottom if vertical
            /// @param widget
            void push_back(Widget*);

            /// @brief insert widget at index
            /// @param widget
            /// @param index
            void insert(Widget*, size_t);

            /// @brief remove all widgets
            void clear();

            /// @brief remove widget
            /// @param widget
            void remove(Widget*);

            /// @brief get number of widgets
            /// @return n
            size_t get_n_items() const;

            /// @brief enable user interaction where a user can click-drag across the mousetrap::GridView to select multiple items
            /// @param enabled true to enable, false otherwise
            void set_enable_rubberband_selection(bool);

            /// @brief get whether user interaction where a user can click-drag across the mousetrap::GridView to select multiple items is enabled
            /// @return true if enabled, false otherwise
            bool get_enable_rubberband_selection() const;

            /// @brief set maximum number of columns
            /// @param n
            void set_max_n_columns(size_t);

            /// @brief get maximum number of columns
            /// @return n
            size_t get_max_n_columns() const;

            /// @brief set minimum number of columns
            /// @param n
            void set_min_n_columns(size_t);

            /// @brief get minimum number of columns
            /// @return n
            size_t get_min_n_columns() const;

            /// @brief get selection model
            /// @return pointer to selection model
            SelectionModel* get_selection_model();

            /// @brief set whether an <tt>activated</tt> signal should be emitted when the user selects the item, as opposed to selecting and activating the item
            /// @param enable true to enable, false otherwise
            void set_single_click_activate(bool);

            /// @brief get whether an <tt>activated</tt> signal is be emitted when the user selects the item, as opposed to selecting and activating the item
            /// @return true if enable, false otherwise
            bool get_single_click_activate() const;

            /// @copydoc mousetrap::Orientable::set_orientation
            void set_orientation(Orientation) override;

            /// @copydoc mousetrap::Orientation::get_orientation
            Orientation get_orientation() const override;

        private:
            static void on_list_item_factory_setup(GtkSignalListItemFactory* self, void* object, GridView* instance);
            static void on_list_item_factory_teardown(GtkSignalListItemFactory* self, void* object, GridView* instance);
            static void on_list_item_factory_bind(GtkSignalListItemFactory* self, void* object, GridView* instance);
            static void on_list_item_factory_unbind(GtkSignalListItemFactory* self, void* object, GridView* instance);

            GtkGridView* _native;
            GtkSignalListItemFactory* _factory;
            GListStore* _list_store;

            SelectionModel* _selection_model;
            GtkSelectionMode _selection_mode;
            GtkOrientation _orientation;
    };
}
