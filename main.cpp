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

    window->set_child(&button);

    window->show();
    window->present();
    window->set_is_focusable(true);
    window->grab_focus();
}

int main()
{
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
    /*
    sf::Window window(sf::VideoMode(800, 600), "OpenGL", sf::Style::Default, sf::ContextSettings(0, 0, 8, 3, 2));
    window.setVerticalSyncEnabled(true);
    window.setActive(true);

    initialize_opengl();

    std::set<Vector2f> test;

    std::deque<Shape> shapes;
    for (size_t x = 0; x < 4; ++x)
    {
        shapes.emplace_back();
        shapes.back().as_rectangle(Vector2f(x / 4.f), Vector2f{0.5, 0.5});
        shapes.back().set_color(HSVA(rand() / float(RAND_MAX), 1, 1, 1));
    }

    std::deque<RenderTask> tasks;
    for (auto& shape : shapes)
    {
        tasks.emplace_back(&shape);
    }

    bool running = true;
    while (running)
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                running = false;
            else if (event.type == sf::Event::Resized)
                glViewport(0, 0, event.size.width, event.size.height);
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (auto& task : tasks)
            task.render();

        glFlush();
        window.display();
    }

    return 0;
     */
}