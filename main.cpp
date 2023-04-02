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

    auto file = FileDescriptor("/home/clem/Workspace/rat_game/main.cpp");
    auto monitor = file.create_monitor();
    monitor.connect_signal_file_changed([](FileMonitor*, FileMonitorEvent event, const FileDescriptor&, const FileDescriptor&){
        std::cout << "test" << std::endl;
    });

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