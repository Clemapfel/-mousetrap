//
// Copyright 2022 Clemens Cords
// Created on 9/2/22 by clem (mail@clemens-cords.com)
//

#include <include/column_view.hpp>
#include <iostream>

namespace mousetrap::detail
{
    #define G_TYPE_COLUMN_VIEW_ITEM (column_view_item_get_type())

    G_DECLARE_FINAL_TYPE (ColumnViewItem, column_view_item, G, COLUMN_VIEW_ITEM, GObject)

    struct _ColumnViewItem
    {
        GObject parent_instance;
        size_t row;
    };

    struct _ColumnViewItemClass
    {
        GObjectClass parent_class;
    };

    G_DEFINE_TYPE (ColumnViewItem, column_view_item, G_TYPE_OBJECT)

    static void column_view_item_finalize (GObject *object)
    {
        auto* self = G_COLUMN_VIEW_ITEM(object);
        G_OBJECT_CLASS (column_view_item_parent_class)->finalize(object);
    }

    static void column_view_item_init(ColumnViewItem* item)
    {
        // noop
    }

    static void column_view_item_class_init(ColumnViewItemClass* klass)
    {
        GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
        gobject_class->finalize = column_view_item_finalize;
    }

    static ColumnViewItem* column_view_item_new(size_t row_i)
    {
        auto* item = (ColumnViewItem*) g_object_new(G_TYPE_COLUMN_VIEW_ITEM, nullptr);
        column_view_item_init(item);
        item->row = row_i;
        return item;
    }
}

namespace mousetrap
{
    ColumnView::ColumnView(SelectionMode mode)
        : WidgetImplementation<GtkColumnView>(GTK_COLUMN_VIEW(gtk_column_view_new(nullptr)))
    {
        _list_store = g_list_store_new(G_TYPE_OBJECT);

        _selection_mode = (GtkSelectionMode) mode;
        if (_selection_mode == GTK_SELECTION_MULTIPLE)
            _selection_model = GTK_SELECTION_MODEL(gtk_multi_selection_new(G_LIST_MODEL(_list_store)));
        else if (_selection_mode == GTK_SELECTION_SINGLE or _selection_mode == GTK_SELECTION_BROWSE)
            _selection_model = GTK_SELECTION_MODEL(gtk_single_selection_new(G_LIST_MODEL(_list_store)));
        else if (_selection_mode == GTK_SELECTION_NONE)
            _selection_model = GTK_SELECTION_MODEL(gtk_no_selection_new(G_LIST_MODEL(_list_store)));

        gtk_column_view_set_model(get_native(), _selection_model);
    }

    void ColumnView::on_list_item_factory_bind(GtkSignalListItemFactory* self, void* object, void* data)
    {
        auto* list_item = GTK_LIST_ITEM(object);
        auto* item = detail::G_COLUMN_VIEW_ITEM(gtk_list_item_get_item(list_item));

        ColumnView::Column* instance = (ColumnView::Column*) data;
        auto* widget = instance->_widgets.size() > item->row ? instance->_widgets.at(item->row) : nullptr;
        gtk_list_item_set_child(list_item, widget != nullptr ? widget->operator GtkWidget*() : nullptr);
    }

    void ColumnView::on_list_item_factory_unbind(GtkSignalListItemFactory* self, void* object, void*)
    {

    }

    void ColumnView::on_list_item_factory_setup(GtkSignalListItemFactory* self, void* object, void*)
    {

    }

    void ColumnView::on_list_item_factory_teardown(GtkSignalListItemFactory* self, void* object, void*)
    {

    }

    ColumnView::Column::Column(ColumnView* owner, const std::string& title)
        : _owner(owner), _title(title)
    {
        _factory = GTK_SIGNAL_LIST_ITEM_FACTORY(gtk_signal_list_item_factory_new());

        g_signal_connect(_factory, "bind", G_CALLBACK(on_list_item_factory_bind), this);
        g_signal_connect(_factory, "unbind", G_CALLBACK(on_list_item_factory_unbind), this);
        g_signal_connect(_factory, "setup", G_CALLBACK(on_list_item_factory_setup), this);
        g_signal_connect(_factory, "teardown", G_CALLBACK(on_list_item_factory_teardown), this);

        _column = gtk_column_view_column_new(title.c_str(), GTK_LIST_ITEM_FACTORY(_factory));
    }

    ColumnView::Column::operator GtkColumnViewColumn*()
    {
        return _column;
    }

    void ColumnView::Column::push_back(Widget* widget)
    {
        _widgets.push_back(widget);
        auto* item = detail::column_view_item_new(_widgets.size() - 1);

        if (_widgets.size() > _owner->_n_rows)
        {
            g_list_store_append(G_LIST_STORE(_owner->_list_store), item);
            _owner->_n_rows++;
        }
    }

    ColumnView::Column* ColumnView::append_column(const std::string& title)
    {
        auto* new_column = _columns.emplace_back(new Column(this, title));
        gtk_column_view_append_column(get_native(), new_column->operator GtkColumnViewColumn*());
        return new_column;
    }
}