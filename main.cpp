#include <SFML/Window.hpp>

#include <mousetrap/include/shape.hpp>
#include <mousetrap/include/render_task.hpp>
#include <mousetrap/include/image.hpp>
#include <mousetrap/include/signals.hpp>

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

    auto instance = Test();
    instance.connect_signal_woof([](auto){
        std::cout << "woof" << std::endl;
    }, nullptr);
    instance.emit_signal_woof();

    initialize_opengl();

    auto shape = Shape();
    shape.as_rectangle({0.25, 0.25}, {0.5, 0.5});

    auto image = Image();
    image.create_from_file("/home/clem/Workspace/mousetrap/resources/icons/bucket_fill.png");

    for (size_t x = 0; x < image.get_size().x; ++x)
        for (size_t y = 0; y < image.get_size().y; ++y)
            if (x == y)
                image.set_pixel(x, y, RGBA(1, 0, 1, 1));

    auto texture = Texture();
    texture.create_from_image(image);
    shape.set_texture(&texture);

    auto task = RenderTask(&shape);

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
        task.render();
        glFlush();
        window.display();
    }

    return 0;
}