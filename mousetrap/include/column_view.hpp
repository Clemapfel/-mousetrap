//
// Copyright (c) Clemens Cords (mail@clemens-cords.com), created 4/10/23
//

#pragma once

#include <include/widget.hpp>
#include <include/menu_model.hpp>
#include <include/gtk_common.hpp>
#include <include/selection_model.hpp>

namespace mousetrap
{
    #ifndef DOXYGEN
    namespace detail
    {
        struct _ColumnViewInternal
        {
            GObject parent;

            GtkColumnView* native;
            GListStore* list_store;

            SelectionModel* selection_model;
            GtkSelectionMode selection_mode;
        };
        using ColumnViewInternal = _ColumnViewInternal;
    }
    #endif

    class ColumnView : public WidgetImplementation<GtkColumnView>
    {
        public:
            class Column
            {
                friend class ColumnView;

                public:
                    /// @brief uninstatiable, use ColumnView::get_column to access individual columns
                    Column() = delete;

                    /// @brief set title of column
                    /// @param title
                    void set_title(const std::string&);

                    /// @brief get title of column
                    /// @return title
                    std::string get_title() const;

                    /// @brief set fixed width of column
                    /// @param width
                    void set_fixed_width(float);

                    /// @brief get fixed width of column
                    /// @return width
                    float get_fixed_with() const;

                    /// @brief set the menumodel to use for the header menu, this menu is accessible by clicked the columns header bar
                    /// @param menu_model
                    void set_header_menu(const MenuModel&);

                    /// @brief set whether column should be visible
                    /// @param visible
                    void set_is_visible(bool);

                    /// @brief get whether column is visible
                    /// @return true if visible, false otherwise
                    bool get_is_visible() const;

                    /// @brief set whether column should be resizable
                    /// @param resizable
                    void set_is_resizable(bool);

                    /// @brief get whether column is resizable
                    /// @return true if resizable, false otherwise
                    bool get_is_resizable() const;

                protected:
                    /// @brief construct as thin wrapper \internal
                    /// @param native native GtkColumnViewColumn
                    Column(GtkColumnViewColumn*);

                private:
                    GtkColumnViewColumn* _native = nullptr;
            };

        public:
            ColumnView(SelectionMode mode = SelectionMode::NONE);

            /// @brief add a column to the end of the column view
            /// @param title header name for the column
            /// @returns pointer to newly created column iterator
            [[nodiscard]] Column push_back_column(const std::string& title);

            /// @brief add a colum to the start of the column view
            /// @param title header name for the column
            /// @returns pointer to newly created column iterator
            [[nodiscard]] Column push_front_column(const std::string& title);

            /// @brief insert a column at given position, inserts at the end if index is out of bounds
            /// @param index index of column to insert a new column after
            /// @param title header name for the column
            /// @returns pointer to newly created column iterator
            [[nodiscard]] Column insert_column(size_t index, const std::string& title);

            /// @brief remove a column from the column view
            /// @param column
            void remove_column(const Column&);

            /// @brief get column at specified position
            /// @param column_i index
            /// @returns column or nullptr if column_i out of bounds
            [[nodiscard]] Column get_column_at(size_t column_i);

            /// @brief get number of columns
            /// @return n
            size_t get_n_columns() const;

            /// TODO
            void append_row(const Widget& widget);

        private:
            static GtkColumnViewColumn* new_column(const std::string& title);
            static void on_list_item_factory_bind(GtkSignalListItemFactory* self, void* object, GtkColumnViewColumn*);
            static void on_list_item_factory_unbind(GtkSignalListItemFactory* self, void* object, GtkColumnViewColumn*);

            detail::ColumnViewInternal* _internal = nullptr;
    };
}
