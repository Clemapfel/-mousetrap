// 
// Copyright 2022 Clemens Cords
// Created on 9/16/22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <include/widget.hpp>
#include <include/label.hpp>
#include <include/signal_component.hpp>

#include <iostream>  // TODO

namespace mousetrap
{
    class DropDown : public WidgetImplementation<GtkDropDown>
    {
        public:
            using ItemID = struct { size_t _value; };

            DropDown();
            ~DropDown();

            template<typename Function_t, typename Data_t>
            [[nodiscard]] ItemID push_back(Widget* list_widget, Widget* label_widget, Function_t, Data_t);

            template<typename Function_t>
            [[nodiscard]] ItemID push_back(Widget* list_widget, Widget* label_widget, Function_t);

            template<typename Function_t, typename Data_t>
            [[nodiscard]] ItemID push_front(Widget* list_widget, Widget* label_widget, Function_t, Data_t);

            template<typename Function_t>
            [[nodiscard]] ItemID push_front(Widget* list_widget, Widget* label_widget, Function_t);

            template<typename Function_t, typename Data_t>
            [[nodiscard]] ItemID insert(size_t i, Widget* list_widget, Widget* label_widget, Function_t, Data_t);

            template<typename Function_t>
            [[nodiscard]] ItemID insert(size_t i, Widget* list_widget, Widget* label_widget, Function_t);

            void remove(ItemID);

            void set_show_arrow(bool);
            bool get_show_arrow() const;

            void set_selected(ItemID);
            ItemID get_selected() const;

            ItemID get_item_at(size_t index) const;

        private:
            static void on_list_factory_bind(GtkSignalListItemFactory* self, void* object, DropDown*);
            static void on_label_factory_bind(GtkSignalListItemFactory* self, void* object, DropDown*);

            static void on_list_factory_unbind(GtkSignalListItemFactory* self, void* object, DropDown*);
            static void on_label_factory_unbind(GtkSignalListItemFactory* self, void* object, DropDown*);

            static void on_selected_item_changed(GtkDropDown*, void*, DropDown*);

            GtkSignalListItemFactory* _list_factory;
            GtkSignalListItemFactory* _label_factory;
            GListStore* _model;

            static inline size_t _current_id = 0;
    };

    namespace detail
    {
        struct _DropDownItem;
        _DropDownItem* drop_down_item_new(Widget* in, Widget* label, DropDown* owner, std::function<void()> f);
    }

    template<typename Function_t, typename Data_t>
    DropDown::ItemID DropDown::push_back(Widget* list_widget, Widget* label_widget, Function_t f_in, Data_t data_in)
    {
        auto id = _current_id++;
        auto* item = detail::drop_down_item_new(
            id,
            list_widget,
            label_widget,
            this,
            [f = f_in, data = data_in](){
            f(data);
        });

        g_list_store_append(_model, item);
        return ItemID{id};
    }

    template<typename Function_t>
    DropDown::ItemID DropDown::push_back(Widget* list_widget, Widget* label_widget, Function_t f_in)
    {
        auto id = _current_id++;
        auto* item = detail::drop_down_item_new(
        id,
        list_widget,
        label_widget,
        this,
        [f = f_in](){
            f();
        });

        g_list_store_append(_model, item);
        return ItemID{id};
    }

    template<typename Function_t, typename Data_t>
    DropDown::ItemID DropDown::push_front(Widget* list_widget, Widget* label_widget, Function_t f_in, Data_t data_in)
    {
        auto id = _current_id++;
        auto* item = detail::drop_down_item_new(
        id,
        list_widget,
        label_widget,
        this,
        [f = f_in, data = data_in](){
            f(data);
        });

        g_list_store_prepend(_model, item);
        return ItemID{id};
    }

    template<typename Function_t>
    DropDown::ItemID DropDown::push_front(Widget* list_widget, Widget* label_widget, Function_t f_in)
    {
        auto id = _current_id++;
        auto* item = detail::drop_down_item_new(
        id,
        list_widget,
        label_widget,
        this,
        [f = f_in](){
            f();
        });

        g_list_store_prepend(_model, item);
        return ItemID{id};
    }

    template<typename Function_t, typename Data_t>
    DropDown::ItemID DropDown::insert(size_t i, Widget* list_widget, Widget* label_widget, Function_t f_in, Data_t data_in)
    {
        if (i >= g_list_model_get_n_items(G_LIST_MODEL(_model)))
            return push_back(list_widget, label_widget, f_in, data_in);

        auto id = _current_id++;
        auto* item = detail::drop_down_item_new(
        id,
        list_widget,
        label_widget,
        this,
        [f = f_in, data = data_in](){
            f(data);
        });

        g_list_store_insert(_model, i, item);
        return ItemID{id};
    }

    template<typename Function_t>
    DropDown::ItemID DropDown::insert(size_t i, Widget* list_widget, Widget* label_widget, Function_t f_in)
    {
        if (i >= g_list_model_get_n_items(G_LIST_MODEL(_model)))
            return push_back(list_widget, label_widget, f_in);

        auto id = _current_id++;
        auto* item = detail::drop_down_item_new(
        id,
        list_widget,
        label_widget,
        this,
        [f = f_in](){
            f();
        });

        g_list_store_insert(_model, i, item);
        return ItemID{id};
    }
}

#include <src/drop_down.inl>

