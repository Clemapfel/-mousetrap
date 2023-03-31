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

inline Window* window = nullptr;
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
    window = new Window(*app);
    window->set_show_menubar(true);

    static auto button = Button();
    button.connect_signal_clicked([](Button* button){
        static auto clipboard = Clipboard(window);
        clipboard.get_string([](Clipboard*, const std::string& str){
            std::cout << FileDescriptor(str).get_uri() << std::endl;
        });
    });


    GtkFileChooserNative* native = gtk_file_chooser_native_new (
        "test",
        window->operator GtkWindow*(),
        GTK_FILE_CHOOSER_ACTION_OPEN,
        "accept",
        "cancel"
    );

    auto* test = gtk_file_chooser_widget_new(GTK_FILE_CHOOSER_ACTION_OPEN);
    gtk_window_set_child(window->operator GtkWindow*(), test);
    //gtk_widget_set_margin_start(GTK_WIDGET(native), 20);
    //gtk_native_dialog_show(GTK_NATIVE_DIALOG(native));

    window->show();
    window->present();
    window->set_is_focusable(true);
    window->grab_focus();
}

int main()
{
    sol::state lua;
    lua.open_libraries(sol::lib::base);

    lua.script("print('bark bark bark!')");

    app = new Application("app.mousetrap");

    app->connect_signal_activate([](Application* app)
    {
        // TODO menubar doesn't show up
    });

    app->connect_signal("startup", startup);

    auto out = app->run();

    delete app;
    delete window;

    return out;
}