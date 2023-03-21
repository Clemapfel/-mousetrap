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
s
        std::vector<Widget*>* widgets;
        std::vector<GtkWidget*>* widget_refs;
    };

    struct _ColumnViewItemClass
    {
        GObjectClass parent_class;
    };

    G_DEFINE_TYPE (ColumnViewItem, column_view_item, G_TYPE_OBJECT)

    static void column_view_item_finalize (GObject *object)
    {
        auto* self = G_COLUMN_VIEW_ITEM(object);

        for (auto* w : *(self->widget_refs))
            g_object_unref(w);

        delete self->widgets;
        delete self->widget_refs;

        G_OBJECT_CLASS (column_view_item_parent_class)->finalize (object);
    }

    static void column_view_item_init(ColumnViewItem* item)
    {
        item->widgets = new std::vector<Widget*>();
        item->widget_refs = new std::vector<GtkWidget*>();
    }

    static void column_view_item_class_init(ColumnViewItemClass* klass)
    {
        GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
        gobject_class->finalize = column_view_item_finalize;
    }

    static ColumnViewItem* column_view_item_new(const std::vector<Widget*>& in)
    {
        auto* item = (ColumnViewItem*) g_object_new(G_TYPE_COLUMN_VIEW_ITEM, nullptr);
        column_view_item_init(item);

        for (auto* w : in)
        {
            item->widgets->push_back(w);
            item->widget_refs->push_back(g_object_ref(w->operator GtkWidget*()));
        }

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

    void ColumnView::on_list_item_factory_bind(GtkSignalListItemFactory* self, void* object, void*)
    {
        auto* list_item = GTK_LIST_ITEM(object);
        auto* object_in = detail::G_COLUMN_VIEW_ITEM(gtk_list_item_get_item(list_item));

        if (rand() / float(RAND_MAX) > 0.8)
            gtk_list_item_set_child(list_item, object_in->widgets->at(0)->operator GtkWidget*());
        else
            gtk_list_item_set_child(list_item, nullptr);
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

        g_signal_connect(_factory, "bind", G_CALLBACK(on_list_item_factory_bind), nullptr);
        g_signal_connect(_factory, "unbind", G_CALLBACK(on_list_item_factory_unbind), nullptr);
        g_signal_connect(_factory, "setup", G_CALLBACK(on_list_item_factory_setup), nullptr);
        g_signal_connect(_factory, "teardown", G_CALLBACK(on_list_item_factory_teardown), nullptr);

        _column = gtk_column_view_column_new(title.c_str(), GTK_LIST_ITEM_FACTORY(_factory));
    }

    ColumnView::Column::operator GtkColumnViewColumn*()
    {
        return _column;
    }

    void ColumnView::Column::push_back(Widget* widget)
    {
        auto* item = detail::column_view_item_new({widget});
        g_list_store_append(G_LIST_STORE(_owner->_list_store), item);
    }

    ColumnView::Column* ColumnView::append_column(const std::string& title)
    {
        auto* new_column = _columns.emplace_back(new Column(this, title));
        gtk_column_view_append_column(get_native(), new_column->operator GtkColumnViewColumn*());
        return new_column;
    }
}