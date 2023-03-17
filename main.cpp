#include <SFML/Window.hpp>

#include <mousetrap/include/shape.hpp>
#include <mousetrap/include/render_task.hpp>
#include <mousetrap/include/image.hpp>
#include <mousetrap/include/signals.hpp>
#include <mousetrap/include/rectangle_shape.hpp>
#include <mousetrap/include/box.hpp>
#include <mousetrap/include/sound.hpp>
#include <mousetrap/include/sound_stream.hpp>

#include <iostream>
using namespace mousetrap;

int main(int argc, char **argv)
{
    auto stream = SoundStream();

    for (size_t i = 0; i < 2; ++i)
    {
        std::cout << "open" << std::endl;
        stream.open();
        std::this_thread::sleep_for(std::chrono::seconds(2));
        stream.close();
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
    std::cout << "quit" << std::endl;
    return 0;
}

/*

int main()
{
    sf::Window window(sf::VideoMode(800, 600), "OpenGL", sf::Style::Default, sf::ContextSettings(
    0, 0, 8, 3, 2
    ));
    window.setVerticalSyncEnabled(true);
    window.setActive(true);

    initialize_opengl();

    std::vector<Shape> shapes;
    std::vector<RenderTask> tasks;
    const size_t n = 10;
    for (size_t x = 0; x < n; ++x)
    {
        shapes.emplace_back();
        std::cout << (x / float(n+1)) << std::endl;
        shapes.back().as_rectangle({x / float(n), x / float(n)}, {0.05, 0.05});
        shapes.back().set_color(HSVA((x+1) / float(n), 1, 1, 1));
    }

    for (auto& shape : shapes)
        tasks.emplace_back(&shape, nullptr, nullptr, BlendMode::NORMAL);

    auto shader = Shader();
    auto transform = GLTransform();

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
        box.push_back(&shape);
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

*/