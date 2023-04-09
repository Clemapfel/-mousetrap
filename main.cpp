#include <mousetrap.hpp>

#include <deque>
#include <iostream>
#include <thread>
#include <memory>

using namespace mousetrap;

///
inline struct State {
    Window window;
}* state = nullptr;

int main()
{
    auto app = Application("mousetrap.debug");

    app.connect_signal_activate([&](Application* app) -> void
    {
        state = new State {
            Window(*app)
        };

        log::set_surpress_debug(MOUSETRAP_DOMAIN, false);

        auto button = Button();
        button.connect_signal_clicked([](Button* button){
            std::cout << "clicked" << std::endl;
        });

        auto motion_controller = MotionEventController();
        motion_controller.connect_signal_motion_enter([](MotionEventController*, float, float){
            std::cout << "enter" << std::endl;
        });

        state->window.add_controller(&motion_controller);
        state->window.connect_signal_close_request([](Window*) -> bool{
            std::cout << "close" << std::endl;
            return false;
        });

        state->window.set_child(&button);
        state->window.present();
    });

    app.connect_signal_shutdown([](Application* app) -> void{
        delete state;
    });

    return app.run();
}