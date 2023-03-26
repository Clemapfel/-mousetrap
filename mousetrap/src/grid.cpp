//
// Created by clem on 3/24/23.
//

#include <include/grid.hpp>
#include <iostream>

namespace mousetrap
{
    Grid::Grid()
        : WidgetImplementation<GtkGrid>(GTK_GRID(gtk_grid_new()))
    {}

    void Grid::insert(Widget* widget, Vector2i row_column_index, size_t n_horizontal_cells, size_t n_vertical_cells)
    {
        if (widget == nullptr)
            return;

        gtk_grid_attach(get_native(), widget->operator GtkWidget*(), row_column_index.x, row_column_index.y, n_horizontal_cells, n_vertical_cells);
    }

    void Grid::remove(Widget* widget)
    {
        if (widget == nullptr)
            return;

        gtk_grid_remove(get_native(), widget->operator GtkWidget*());
    }

    Vector2i Grid::get_position(Widget* widget) const
    {
        if (widget == nullptr)
        {
            std::cerr << "[ERROR] IN Grid::get_position: Querying position of a nullptr" << std::endl;
            return Vector2i();
        }

        int column, row, width, height;
        gtk_grid_query_child(get_native(), widget->operator GtkWidget *(), &column, &row, &width, &height);
        return Vector2i(column, row);
    }

    Vector2ui Grid::get_bounds(Widget* widget) const
    {
        if (widget == nullptr)
            return Vector2i(0, 0);

        int column, row, width, height;
        gtk_grid_query_child(get_native(), widget->operator GtkWidget *(), &column, &row, &width, &height);
        return Vector2ui(width, height);
    }

    void Grid::insert_row_at(int index)
    {
        gtk_grid_insert_row(get_native(), index);
    }

    void Grid::remove_row_at(int index)
    {
        gtk_grid_remove_row(get_native(), index);
    }

    void Grid::insert_column_at(int index)
    {
        gtk_grid_insert_column(get_native(), index);
    }

    void Grid::remove_column_at(int index)
    {
        gtk_grid_remove_column(get_native(), index);
    }

    void Grid::set_column_spacing(float v)
    {
        gtk_grid_set_column_spacing(get_native(), v);
    }

    float Grid::get_column_spacing() const
    {
        return gtk_grid_get_column_homogeneous(get_native());
    }

    void Grid::set_row_spacing(float v)
    {
        gtk_grid_set_row_spacing(get_native(), v);
    }

    float Grid::get_row_spacing() const
    {
        return gtk_grid_get_row_spacing(get_native());
    }

    void Grid::set_rows_homogenous(bool b)
    {
        gtk_grid_set_row_homogeneous(get_native(), b);
    }

    bool Grid::get_rows_homogenous() const
    {
        return gtk_grid_get_row_homogeneous(get_native(), b);
    }

    void Grid::set_columns_homogenous(bool b)
    {
        gtk_grid_set_column_homogeneous(get_native(), b);
    }

    bool Grid::get_columns_homogenous() const
    {
        return gtk_grid_get_column_homogeneous(get_native());
    }

    void Grid::set_orientation(Orientation orientation)
    {
        gtk_orientable_set_orientation(GTK_ORIENTABLE(get_native()), (GtkOrientation) orientation);
    }

    Orientation Grid::get_orientation() const
    {
        return (Orientation) gtk_orientable_get_orientation(GTK_ORIENTABLE(get_native()));
    }
}
