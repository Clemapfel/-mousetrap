#include <SFML/Window.hpp>

#include <mousetrap/include/shape.hpp>
#include <mousetrap/include/render_task.hpp>
#include <mousetrap/include/image.hpp>
#include <mousetrap/include/signals.hpp>
#include <mousetrap/include/rectangle_shape.hpp>
#include <mousetrap/include/box.hpp>

#include <iostream>

using namespace mousetrap;

struct Test : public HasWoofSignal
{};

int main()
{
    sf::Window window(sf::VideoMode(800, 600), "OpenGL", sf::Style::Default, sf::ContextSettings(
    0, 0, 8, 3, 2
    ));
    window.setVerticalSyncEnabled(true);
    window.setActive(true);

    initialize_opengl();

    std::deque<RectangleShape> shapes;
    for (size_t x = 0; x < 4; ++x)
    {
        shapes.emplace_back(Vector2f{0.25, 0.25}, Vector2f{0.5, 0.5});
        shapes.back().set_color(HSVA(rand() / float(RAND_MAX), 1, 1, 1));
    }

    Box box = Box(Orientation::HORIZONTAL);
    box.set_reformatting_blocked(true);

    std::deque<RenderTask> tasks;
    for (auto& shape : shapes)
    {
        tasks.push_back(RenderTask(shape.get_shape()));
        //box.push_back(&shape);
    }

    box.set_centroid({0.5, 0.5});
    box.set_size({1, 1});
    box.set_reformatting_blocked(false);

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
}