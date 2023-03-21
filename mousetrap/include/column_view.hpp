//
// Copyright 2022 Clemens Cords
// Created on 9/2/22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <include/widget.hpp>
#include <include/selection_model.hpp>

#include <deque>

namespace mousetrap
{
    namespace detail { struct _ColumnViewItem; }

    class ColumnView : public WidgetImplementation<GtkColumnView>
    {
        class Column
        {
            friend class ColumnView;

            public:
                operator GtkColumnViewColumn*();
                void push_back(Widget*);
                void push_front(Widget*);
                void insert(size_t, Widget*);
                void remove(size_t);

                Widget* get_widget_at(size_t row_i);

            protected:
                Column(ColumnView*, const std::string&);

            private:
                ColumnView* _owner = nullptr;

                std::string _title;
                GtkSignalListItemFactory* _factory = nullptr;
                GtkColumnViewColumn* _column = nullptr;
                std::deque<Widget*> _widgets;
        };

        public:
            using Column = Column;

            ColumnView(SelectionMode mode = SelectionMode::NONE);
            [[nodiscard]] Column* push_back_column(const std::string& title);
            [[nodiscard]] Column* push_front_column(const std::string& title);
            [[nodiscard]] Column* insert_column(size_t index, const std::string& title);

            void remove_column(Column*);
            void clear();

            Column* get_column_at(size_t column_i);

            void set_enable_rubberband_selection(bool);
            bool get_enable_rubberband_selection() const;

            void set_show_row_separators(bool);
            bool get_show_row_separators() const;

            void set_show_column_separators(bool);
            bool get_show_column_separators() const;

            void set_single_click_activate(bool);
            bool get_single_click_activate() const;

            SelectionModel* get_selection_model();

            size_t get_n_rows() const;
            size_t get_n_columns() const;

        private:
            SelectionModel* _selection_model = nullptr;
            GtkSelectionMode _selection_mode;

            GListStore* _list_store = nullptr;

            static void on_list_item_factory_setup(GtkSignalListItemFactory* self, void* object, void*);
            static void on_list_item_factory_teardown(GtkSignalListItemFactory* self, void* object, void*);
            static void on_list_item_factory_bind(GtkSignalListItemFactory* self, void* object, void*);
            static void on_list_item_factory_unbind(GtkSignalListItemFactory* self, void* object, void*);

            std::deque<Column*> _columns;
            size_t _n_rows = 0;
    };
}