//
// Copyright 2022 Clemens Cords
// Created on 9/2/22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <include/widget.hpp>
#include <include/selectable.hpp>

#include <deque>

namespace mousetrap
{
    #ifndef DOXYGEN
    namespace detail
    {
        struct _ColumnViewItem;

        struct _ColumnViewColumnInternal;
        using ColumnViewColumnInternal = _ColumnViewColumnInternal;

        struct _ColumnViewInternal;
        using ColumnViewInternal = _ColumnViewInternal;
    }
    #endif

    /// @brief display a list of items separator by columns
    class ColumnView : public WidgetImplementation<GtkColumnView>, public Selectable
    {
        class Column
        {
            friend class ColumnView;

            public:
                /// @brief expose as GtkColumnViewColumn \internal
                operator GtkColumnViewColumn*();

                /// @brief replace the widget at position, if row_i out of bounds, TODO
                /// @param row_i row index
                /// @param widget
                void set_widget_at(size_t row_i, Widget*);

                /// @brief get widget at position, or nullptr if position is out of bounds
                /// @param row_i row index
                /// @return widget, may be nullptr
                Widget* get_widget_at(size_t row_i) const;

                void remove(size_t row_i);

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

            /// @brief construct
            /// @param mode selection mode
            ColumnView(SelectionMode mode = SelectionMode::NONE);

            /// @brief add a column to the end of the column view
            /// @param title header name for the column
            /// @returns pointer to newly created column iterator
            [[nodiscard]] Column* push_back_column(const std::string& title);

            /// @brief add a colum to the start of the column view
            /// @param title header name for the column
            /// @returns pointer to newly created column iterator
            [[nodiscard]] Column* push_front_column(const std::string& title);

            /// @brief insert a column at given position, inserts at the end if index is out of bounds
            /// @param index index of column to insert a new column after
            /// @param title header name for the column
            /// @returns pointer to newly created column iterator
            [[nodiscard]] Column* insert_column(size_t index, const std::string& title);

            /// @brief remove a column from the column view
            void remove_column(Column*);

            /// @brief remove all columns from the column view
            void clear();

            /// @brief add a row to the end
            /// @param widget vector if widget, if vector has more entires than number columns, overlap is ignored. If vector has less entires than number of columns, it will be backfilled with nullptr
            void push_back_row(const std::vector<Widget*>&);

            /// @brief add a row to the start
            /// @param widget vector if widget, if vector has more entires than number columns, overlap is ignored. If vector has less entires than number of columns, it will be backfilled with nullptr
            void push_front_row(const std::vector<Widget*>&);

            /// @brief add a row at specified position
            /// @param widget vector if widget, if vector has more entires than number columns, overlap is ignored. If vector has less entires than number of columns, it will be backfilled with nullptr
            void insert_row(size_t, const std::vector<Widget*>&);

            /// @brief remove row at specified position, does nothing if row index is out of bounds
            /// @param widget vector if widget, if vector has more entires than number columns, overlap is ignored. If vector has less entires than number of columns, it will be backfilled with nullptr
            void remove_row(size_t);

            /// @brief get column at specified position
            /// @param column_i index
            /// @returns column or nullptr if column_i out of bounds
            Column* get_column_at(size_t column_i);

            /// @brief set whether the user is able to selecte multiple items by click-dragging
            /// @param b true if this type of selection should be enabled, false otherwise
            void set_enable_rubberband_selection(bool);

            /// @brief get whether the user is able to selecte multiple items by click-dragging
            /// @return true if this type of selection should be enabled, false otherwise
            bool get_enable_rubberband_selection() const;

            /// @brief set whether rows should be separated
            /// @param b true if separators should be visible, false otherwise
            void set_show_row_separators(bool);

            /// @brief get whether row separators are shown
            /// @return true if separators are visible, false otherwise
            bool get_show_row_separators() const;

            /// @brief set whether columns should be separated
            /// @param b true if spearators should be visible, false otherwise
            void set_show_column_separators(bool);

            /// @brief get whether row separators are shown
            /// @return true if separators are visible, false otherwise
            bool get_show_column_separators() const;

            /// @brief set whether the column views seletion model should emit an activate signal when the user clicks on an item in the view once
            /// @param b true if signal emissions this way should be allowed, false otherwise
            void set_single_click_activate(bool);

            /// @brief get whether the column views seletion model should emit an activate signal when the user clicks on an item in the view once
            /// @param b true if signal emissions this way should be allowed, false otherwise
            bool get_single_click_activate() const;

            /// @brief expose the selection model
            /// @return selection model
            SelectionModel* get_selection_model() override;

            /// @brief get number of rows
            /// @return size_t
            size_t get_n_rows() const;

            /// @brief get number of columns
            /// qreturn size_t
            size_t get_n_columns() const;

        private:
            void resize_list();

            SelectionModel* _selection_model = nullptr;
            GtkSelectionMode _selection_mode;

            GListStore* _list_store = nullptr;

            static void on_list_item_factory_bind(GtkSignalListItemFactory* self, void* object, void*);
            static void on_list_item_factory_unbind(GtkSignalListItemFactory* self, void* object, void*);

            std::deque<Column*> _columns;
            size_t _n_rows = 0;
    };
}