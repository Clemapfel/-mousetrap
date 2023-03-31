#include <SFML/Window.hpp>

#include <mousetrap/include/shape.hpp>
#include <mousetrap/include/render_task.hpp>
#include <mousetrap/include/image.hpp>
#include <mousetrap/include/sound.hpp>
#include <mousetrap/include/application.hpp>
#include <mousetrap/include/window.hpp>
#include <mousetrap/include/motion_event_controller.hpp>
#include <mousetrap/include/label.hpp>
#include <mousetrap/include/text_view.hpp>
#include <mousetrap/include/box.hpp>
#include <mousetrap/include/frame.hpp>
#include <mousetrap/include/button.hpp>
#include <mousetrap/include/list_view.hpp>
#include <mousetrap/include/column_view.hpp>
#include <mousetrap/include/popover_menu_button.hpp>
#include <mousetrap/include/stack.hpp>
#include <mousetrap/include/notebook.hpp>
#include <mousetrap/include/spin_button.hpp>
#include <mousetrap/include/drop_down.hpp>
#include <mousetrap/include/scale.hpp>
#include <mousetrap/include/file_system.hpp>
#include <mousetrap/include/spinner.hpp>
#include <mousetrap/include/grid_view.hpp>
#include <mousetrap/include/image_display.hpp>
#include <mousetrap/include/scrolled_window.hpp>
#include <mousetrap/include/icon.hpp>
#include <mousetrap/include/log.hpp>
#include <mousetrap/include/file_monitor.hpp>
#include <mousetrap/include/separator.hpp>
#include <mousetrap/include/aspect_frame.hpp>
#include <mousetrap/include/clipboard.hpp>

#include <deque>
#include <iostream>

#define SOL_ALL_SAFETIES_ON 1

extern "C" {
#include <lua.h>
}

#include <sol2/include/sol/sol.hpp>

using namespace mousetrap;

inline Window* app_window = nullptr;
inline Application* app = nullptr;

inline Action* action;

static void log_handler(const gchar* log_domain, GLogLevelFlags flags, const char* message, gpointer data)
{
    std::cout << "[HANDLED] " << message << std::endl;
}

static GLogWriterOutput log_writer(GLogLevelFlags log_level, const GLogField* fields, gsize n_fields, gpointer user_data)
{
    g_log_writer_standard_streams(log_level, fields, n_fields, user_data);
    return G_LOG_WRITER_HANDLED;
}

static void startup(GApplication*)
{
    app_window = new Window(*app);
    app_window->set_show_menubar(true);

    static GtkWidget *window = NULL;
    GtkWidget *vbox;
    GtkWidget *hbox;
    GtkWidget *box;
    GtkWidget *label;
    GtkWidget *entry;
    GtkWidget *searchbar;
    GtkWidget *button;
    GtkWidget *header;

    window = app_window->operator GtkWidget*();
    gtk_window_set_title (GTK_WINDOW (window), "Type to Search");
    gtk_window_set_resizable (GTK_WINDOW (window), FALSE);
    gtk_widget_set_size_request (window, 200, -1);
    g_object_add_weak_pointer (G_OBJECT (window), (gpointer *)&window);

    header = gtk_header_bar_new ();
    gtk_window_set_titlebar (GTK_WINDOW (window), header);

    vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
    gtk_window_set_child (GTK_WINDOW (window), vbox);

    entry = gtk_search_entry_new ();
    gtk_widget_set_halign (entry, GTK_ALIGN_CENTER);
    searchbar = gtk_search_bar_new ();
    gtk_search_bar_connect_entry (GTK_SEARCH_BAR (searchbar), GTK_EDITABLE (entry));
    gtk_search_bar_set_show_close_button (GTK_SEARCH_BAR (searchbar), FALSE);
    gtk_search_bar_set_child (GTK_SEARCH_BAR (searchbar), entry);
    gtk_box_append (GTK_BOX (vbox), searchbar);

    /* Hook the search bar to key presses */
    gtk_search_bar_set_key_capture_widget (GTK_SEARCH_BAR (searchbar), window);

    box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 18);
    gtk_widget_set_margin_start (box, 18);
    gtk_widget_set_margin_end (box, 18);
    gtk_widget_set_margin_top (box, 18);
    gtk_widget_set_margin_bottom (box, 18);
    gtk_box_append (GTK_BOX (vbox), box);

    /* Toggle button */
    button = gtk_toggle_button_new ();
    gtk_button_set_icon_name (GTK_BUTTON (button), "system-search-symbolic");
    g_object_bind_property (button, "active",
                            searchbar, "search-mode-enabled",
                            G_BINDING_BIDIRECTIONAL);
    gtk_header_bar_pack_end (GTK_HEADER_BAR (header), button);

    /* Result */
    hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_append (GTK_BOX (box), hbox);

    label = gtk_label_new ("Searching for:");
    gtk_label_set_xalign (GTK_LABEL (label), 0.0);
    gtk_box_append (GTK_BOX (hbox), label);

    label = gtk_label_new ("");
    gtk_box_append (GTK_BOX (hbox), label);

    app_window->show();
    app_window->present();
    app_window->set_is_focusable(true);
    app_window->grab_focus();
}

int main()
{
    app = new Application("app.mousetrap");
    app->connect_signal("activate", startup);

    auto out = app->run();

    delete app;
    delete app_window;

    return out;
}