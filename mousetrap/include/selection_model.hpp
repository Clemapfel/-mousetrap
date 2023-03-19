//
// Copyright 2022 Clemens Cords
// Created on 9/30/22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <include/signal_emitter.hpp>
#include <include/signal_component.hpp>

namespace mousetrap
{
    enum class SelectionMode
    {
        NONE = GTK_SELECTION_NONE,
        SINGLE = GTK_SELECTION_SINGLE,
        MULTIPLE = GTK_SELECTION_MULTIPLE,
    };

    class SelectionModel : public SignalEmitter,
        HAS_SIGNAL(SelectionModel, selection_changed)
    {
        public:
            SelectionModel() = delete;
            ~SelectionModel();
            operator GtkSelectionModel*();
            operator GObject*() override;

            std::vector<size_t> get_selection();
            void select_all();
            void unselect_all();

            void select(size_t i, bool unselect_others = true);
            void unselect(size_t i);

        protected:
            SelectionModel(GtkSelectionModel*);

            GtkSelectionModel* _native;
    };

    struct MultiSelectionModel : public SelectionModel
    {
        MultiSelectionModel(GListModel*);
    };

    struct SingleSelectionModel : public SelectionModel
    {
        SingleSelectionModel(GListModel*);
    };

    struct NoSelectionModel : public SelectionModel
    {
        NoSelectionModel(GListModel*);
    };
}

