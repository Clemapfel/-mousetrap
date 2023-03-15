//
// Created by clem on 3/15/23.
//

#include <include/widget.hpp>
#include <iostream>

namespace mousetrap
{
    Widget::Widget()
    {}

    Widget* Widget::get_parent() const
    {
        return _parent;
    }

    void Widget::set_parent(Widget* parent)
    {
        if (_parent != nullptr)
            _parent->remove_child(this);

        _parent = parent;
    }

    void Widget::append_child(Widget* child)
    {
        _children.push_back(child);

        if (not _reformatting_blocked)
            reformat();
    }

    void Widget::prepend_child(Widget* child)
    {
        _children.push_front(child);

        if (not _reformatting_blocked)
            reformat();
    }

    void Widget::remove_child(Widget* widget)
    {
        for (auto it = _children.begin(); it != _children.end(); ++it)
        {
            if (*it == widget)
            {
                _children.erase(it);
                widget->set_parent(nullptr);
                return;
            }
        }

        std::cerr << "[WARNING] In Widget::remove_child: Widget " << widget << " is not a child of this (" << this << ")" << std::endl;
    }

    bool Widget::has_child(Widget* widget)
    {
        for (auto it = _children.begin(); it != _children.end(); ++it)
        {
            if (*it == widget)
                return true;
        }

        return false;
    }

    const std::deque<Widget*>& Widget::get_children() const
    {
        return _children;
    }

    void Widget::set_reformatting_blocked(bool next)
    {
        auto before = _reformatting_blocked;
        _reformatting_blocked = next;

        if (before == false and next == true)
            reformat();
    }
}