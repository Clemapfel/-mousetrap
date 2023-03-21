//
// Copyright 2022 Clemens Cords
// Created on 9/2/22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <include/widget.hpp>
#include <include/selection_model.hpp>

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

            protected:
                Column(ColumnView*, const std::string&);

            private:
                ColumnView* _owner = nullptr;

                std::string _title;
                GtkSignalListItemFactory* _factory = nullptr;
                GtkColumnViewColumn* _column = nullptr;
        };

        public:
            ColumnView(SelectionMode mode = SelectionMode::NONE);
            [[nodiscard]] Column* append_column(const std::string& title);

        //private:
            GtkSelectionModel* _selection_model = nullptr;
            GtkSelectionMode _selection_mode;

            GListStore* _list_store = nullptr;

            static void on_list_item_factory_setup(GtkSignalListItemFactory* self, void* object, void*);
            static void on_list_item_factory_teardown(GtkSignalListItemFactory* self, void* object, void*);
            static void on_list_item_factory_bind(GtkSignalListItemFactory* self, void* object, void*);
            static void on_list_item_factory_unbind(GtkSignalListItemFactory* self, void* object, void*);

            std::vector<Column*> _columns;
    };
}