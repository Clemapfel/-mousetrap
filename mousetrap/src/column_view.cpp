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
            _selection_model = new SelectionModel(GTK_SELECTION_MODEL(gtk_multi_selection_new(G_LIST_MODEL(_list_store))));
        else if (_selection_mode == GTK_SELECTION_SINGLE or _selection_mode == GTK_SELECTION_BROWSE)
            _selection_model = new SelectionModel(GTK_SELECTION_MODEL(gtk_single_selection_new(G_LIST_MODEL(_list_store))));
        else if (_selection_mode == GTK_SELECTION_NONE)
            _selection_model = new SelectionModel(GTK_SELECTION_MODEL(gtk_no_selection_new(G_LIST_MODEL(_list_store))));

        gtk_column_view_set_model(get_native(), _selection_model->operator GtkSelectionModel*());
        gtk_column_view_set_reorderable(get_native(), FALSE);
    }

    void ColumnView::on_list_item_factory_bind(GtkSignalListItemFactory* self, void* object, void* data)
    {
        auto* list_item = GTK_LIST_ITEM(object);
        auto* item = detail::G_COLUMN_VIEW_ITEM(gtk_list_item_get_item(list_item));

        ColumnView::Column* instance = (ColumnView::Column*) data;
        auto* widget = instance->get_widget_at(item->row);
        auto* gtk_widget = widget != nullptr ? widget->operator GtkWidget*() : nullptr;
        if (gtk_widget != nullptr)
            gtk_widget_unparent(gtk_widget);

        gtk_list_item_set_child(list_item, gtk_widget);
    }

    void ColumnView::on_list_item_factory_unbind(GtkSignalListItemFactory* self, void* object, void* data)
    {
        auto* list_item = GTK_LIST_ITEM(object);
        auto* item = detail::G_COLUMN_VIEW_ITEM(gtk_list_item_get_item(list_item));
        gtk_list_item_set_child(list_item, nullptr);
    }

    void ColumnView::on_list_item_factory_setup(GtkSignalListItemFactory* self, void* object, void*)
    {}

    void ColumnView::on_list_item_factory_teardown(GtkSignalListItemFactory* self, void* object, void*)
    {}

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

    void ColumnView::Column::set_widget_at(size_t row_i, Widget* widget)
    {
        while (_widgets.size() <= row_i)
            _widgets.emplace_back(nullptr);

        _widgets.at(row_i) = widget;

        if (row_i >= _owner->_n_rows)
            _owner->resize_list();

        g_list_model_items_changed(G_LIST_MODEL(_owner->_list_store), row_i, 0, 0);
    }

    Widget* ColumnView::Column::get_widget_at(size_t row_i) const
    {
        if (row_i >= _widgets.size())
            return nullptr;
        else
            return _widgets.at(row_i);
    }

    void ColumnView::Column::remove(size_t row_i)
    {
        if (row_i >= _widgets.size())
            return;

        _widgets.at(row_i) = nullptr;

        if (row_i == _owner->_n_rows)
            _owner->resize_list();
    }

    void ColumnView::resize_list()
    {
        size_t max_length = 0;
        for (auto* column : _columns)
            if (column->_widgets.size() > max_length)
                max_length = column->_widgets.size();

        _n_rows = max_length;

        while (g_list_model_get_n_items(G_LIST_MODEL(_list_store)) < max_length)
        {
            auto* item = detail::column_view_item_new(g_list_model_get_n_items(G_LIST_MODEL(_list_store)) - 1);
            g_list_store_append(G_LIST_STORE(_list_store), item);
        }

        while (g_list_model_get_n_items(G_LIST_MODEL(_list_store)) >= max_length)
        {
            g_list_store_remove(_list_store, g_list_model_get_n_items(G_LIST_MODEL(_list_store))-1);
        }
    }

    ColumnView::Column* ColumnView::push_back_column(const std::string& title)
    {
        auto* new_column = _columns.emplace_back(new Column(this, title));
        gtk_column_view_append_column(get_native(), new_column->operator GtkColumnViewColumn*());
        return new_column;
    }

    ColumnView::Column* ColumnView::push_front_column(const std::string& title)
    {
        auto* new_column = _columns.emplace_front(new Column(this, title));
        gtk_column_view_insert_column(get_native(), 0, new_column->operator GtkColumnViewColumn*());
        return new_column;
    }

    ColumnView::Column* ColumnView::insert_column(size_t index, const std::string& title)
    {
        if (index > _columns.size())
            index = _columns.size() == 0 ? 0 : _columns.size() -1;

        auto* new_column = _columns.insert(_columns.begin() + index, new Column(this, title)).operator*();
        gtk_column_view_insert_column(get_native(), index, new_column->operator GtkColumnViewColumn*());
        return new_column;
    }

    void ColumnView::remove_column(Column* column)
    {
        gtk_column_view_remove_column(get_native(), column->operator GtkColumnViewColumn*());

        for (auto it = _columns.begin(); it != _columns.end(); ++it)
        {
            if (*it == column)
            {
                _columns.erase(it);
                break;
            }
        }
    }

    void ColumnView::clear()
    {
        for (auto* column : _columns)
            gtk_column_view_remove_column(get_native(), column->operator GtkColumnViewColumn*());

        _columns.clear();
    }

    void ColumnView::push_back_row(const std::vector<Widget*>& widgets)
    {
        for (size_t i = 0; i < get_n_columns(); ++i)
        {
            if (i >= widgets.size())
                get_column_at(i)->set_widget_at(_n_rows - 1, nullptr);
            else
                get_column_at(i)->set_widget_at(_n_rows - 1, widgets.at(i));
        }
    }

    void ColumnView::push_front_row(const std::vector<Widget*>& widgets)
    {
        for (size_t i = 0; i < get_n_columns(); ++i)
        {
            if (i >= widgets.size())
                get_column_at(i)->set_widget_at(0, nullptr);
            else
                get_column_at(i)->set_widget_at(0, widgets.at(i));
        }
    }

    void ColumnView::insert_row(size_t index, const std::vector<Widget*>& widgets)
    {
        for (size_t i = 0; i < get_n_columns(); ++i)
        {
            if (i >= widgets.size())
                get_column_at(i)->set_widget_at(index, nullptr);
            else
                get_column_at(i)->set_widget_at(index, widgets.at(i));
        }
    }

    void ColumnView::set_enable_rubberband_selection(bool b)
    {
        gtk_column_view_set_enable_rubberband(get_native(), b);
    }

    bool ColumnView::get_enable_rubberband_selection() const
    {
        return gtk_column_view_get_enable_rubberband(get_native());
    }

    void ColumnView::set_show_row_separators(bool b)
    {
        gtk_column_view_set_show_row_separators(get_native(), true);
    }

    bool ColumnView::get_show_row_separators() const
    {
        return gtk_column_view_get_show_row_separators(get_native());
    }

    void ColumnView::set_show_column_separators(bool b)
    {
        gtk_column_view_set_show_column_separators(get_native(), b);
    }

    bool ColumnView::get_show_column_separators() const
    {
        return gtk_column_view_get_show_column_separators(get_native());
    }

    SelectionModel* ColumnView::get_selection_model()
    {
        return _selection_model;
    }

    size_t ColumnView::get_n_rows() const
    {
        return g_list_model_get_n_items(G_LIST_MODEL(_list_store));
    }

    size_t ColumnView::get_n_columns() const
    {
        return _columns.size();
    }

    ColumnView::Column* ColumnView::get_column_at(size_t column_i)
    {
        auto* gtk_column = g_list_model_get_item(gtk_column_view_get_columns(get_native()), column_i);
        if (gtk_column == nullptr)
            return nullptr;
        else
        {
            for (auto* column : _columns)
                if (column->_column == gtk_column)
                    return column;
        }

        assert(false && "In ColumnView::get_column_at: unreachable reached");
        return nullptr;
    }
}