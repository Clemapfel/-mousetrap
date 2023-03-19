//
// Created by clem on 3/19/23.
//

#include <include/text_view.hpp>

namespace mousetrap
{
    TextView::TextView()
        : WidgetImplementation<GtkTextView>(GTK_TEXT_VIEW(gtk_text_view_new()))
    {}
}