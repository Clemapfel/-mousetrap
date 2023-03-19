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

#include <deque>
#include <iostream>

using namespace mousetrap;

inline Window* window = nullptr;
inline Application* app = nullptr;

#define test(signal_name, function) \
template<typename Owner_t>                                    \
struct has_##signal_name##_signal {  \
    int test_01() {                \
        return 1234;                \
    }\
    function             \
};\

test(name, std::string wrapper(Owner_t, std::string str) {
    return str;
})

static void startup(GApplication*)
{
    window = new Window(*app);

    auto action = Action("global.test_action");
    action.set_function([](){
        std::cout << "test" << std::endl;
    });
    action.add_shortcut("<Control>c");

    auto instance = has_name_signal<std::string>();
    instance.wrapper("abc", "def");

    window->connect_signal_close_request([](Window*) -> bool {
        std::cout << "close" << std::endl;
        return false;
    });

    auto* motion_controller = new MotionEventController();
    motion_controller->connect_signal_motion([](MotionEventController*, double x, double y) -> void {
        std::cout << x << " " << y << std::endl;
    }, nullptr);
    window->add_controller(motion_controller);

    auto label = Label();
    label.set_text("askdbaöusdböaosudöasoubdaibslidbaisubdöaiusbdöasubdöasoubdöaoudbaöowbföwoubÖOUBöoaubwöUOBAFÖOUWBöAOUWBFÖOAWUBFöOAWUbÖFOUWABöoAWUBFöOAWUBFÖOAUWFbÖOWUBÖAOUWfbÖAOUWFbÖAOWUFbÖWAOUFbÖOAUWBFöoWUFBWÖOWAUFBÖOWUAB");
    label.set_wrap_mode(LabelWrapMode::ONLY_ON_CHAR);
    window->set_child(&label);

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