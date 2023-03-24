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
    class GridView : public WidgetImplementation<GtkGridView>, public Orientable,
        HAS_SIGNAL(GridView, activate)
    {
        public:
            GridView(Orientation = Orientation::VERTICAL, SelectionMode = SelectionMode::NONE);

            void push_front(Widget*);
            void push_back(Widget*);
            void insert(Widget*, size_t);

            void clear();

            void remove(Widget*);
            size_t get_n_items() const;

            void set_enable_rubberband_selection(bool);
            bool get_enable_rubberband_selection() const;

            void set_max_n_columns(size_t);
            size_t get_max_n_columns() const;

            void set_min_n_columns(size_t);
            size_t get_min_n_columns() const;

            SelectionModel* get_selection_model();

            void set_single_click_activate(bool);
            bool get_single_click_activate() const;

            void set_orientation(Orientation) override;
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
