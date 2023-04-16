#include <mousetrap.hpp>

#include <deque>
#include <iostream>
#include <thread>
#include <memory>

using namespace mousetrap;

int main()
{
    std::cout << GTK_MINOR_VERSION << std::endl;
    return 0;

    auto app = Application("app.debug");
    app.connect_signal_activate([](Application* app){

        static auto window = Window(*app);
        static auto controller = ShortcutController();

        static auto action = Action("test");
        action.set_function([](ShortcutController* controller){
           std::cout << "called" << std::endl;
           controller->remove_action(action);
        }, &controller);
        action.add_shortcut("<Control>c");
        app->add_action(action);

        controller.add_action(action);
        window.add_controller(controller);
        window.present();
    });

    return app.run();
}