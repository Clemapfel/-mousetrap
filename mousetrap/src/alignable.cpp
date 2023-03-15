//
// Created by clem on 3/15/23.
//

#include <include/alignable.hpp>

namespace mousetrap
{
    void Alignable::set_alignment(Alignment alignment)
    {
        set_horizontal_alignment(alignment);
        set_vertical_alignment(alignment);
    }

    void Alignable::set_horizontal_alignment(Alignment alignment)
    {
        _horizontal_alignment = alignment;
    }

    void Alignable::set_vertical_alignment(Alignment alignment)
    {
        _vertical_alignment = alignment;
    }

    Alignment Alignable::get_horizontal_alignment() const
    {
        return _horizontal_alignment;
    }

    Alignment Alignable::get_vertical_alignment() const
    {
        return _vertical_alignment;
    }
}