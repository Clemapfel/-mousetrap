#include <SFML/Window.hpp>

#include <mousetrap/include/shape.hpp>
#include <mousetrap/include/render_task.hpp>
#include <mousetrap/include/image.hpp>
#include <mousetrap/include/sound.hpp>
#include <mousetrap/include/sound_stream.hpp>
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

#include <deque>
#include <iostream>

using namespace mousetrap;

inline Window* window = nullptr;
inline Application* app = nullptr;

inline Action* action;

static void on_select(GtkSelectionModel* model, int32_t i, int32_t n)
{
    std::cout << "postition: " << i << " | " << "n: " << n << std::endl;
}

static void startup(GApplication*)
{
    window = new Window(*app);

    action = new Action("global.test_action");
    action->set_function([](auto test2){
        std::cout << test2 << std::endl;
    }, "testest");
    action->add_shortcut("<Control>c");
    app->add_action(action);

    auto menu_model = MenuModel();
    menu_model.add_action("test", *action);

    window->connect_signal_close_request([](Window*) -> bool {
        std::cout << "close" << std::endl;
        return false;
    });

    auto* tree = new ColumnView();

    ColumnView::Column* special_column;
    for (size_t column_i = 0; column_i < 5; ++column_i)
    {
        auto* column = tree->push_back_column(std::to_string(column_i));

        for (size_t i = 0; i < column_i; ++i)
            column->push_back(new Label(std::to_string(int((rand() / float(RAND_MAX)) * 100u))));

        special_column = column;
    }

    auto* box = new Box(Orientation::VERTICAL);
    auto* button = new Button();
    button->connect_signal_clicked([](Button*, ColumnView::Column* special_column){
        special_column->push_front(new Label("new"));
    }, special_column);

    box->push_back(tree);
    box->push_back(button);
    window->set_child(box);

    window->show();
    window->present();
    window->set_focusable(true);
    window->grab_focus();
}

int main()
{
    app = new Application();
    app->connect_signal_activate([](Application* app){
        std::cout << "activate " << app << std::endl;
    });

    app->connect_signal("startup", startup);

    std::cout << app << std::endl;
    return app->run();

    delete app;
    delete window;

    return 0;
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