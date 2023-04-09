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

        auto action = Action("test");
        action.set_function([](){
            std::cout << "action" << std::endl;
        });
        app->add_action(action);

        auto button = Button();
        button.set_action(action);
        button.set_tick_callback([](FrameClock){
            std::cout << "called" << std::endl;
            return TickCallbackResult::CONTINUE;
        });

        auto motion_controller = MotionEventController();
        motion_controller.connect_signal_motion_enter([](MotionEventController*, float, float){
            std::cout << "enter" << std::endl;
        });

        state->window.add_controller(motion_controller);
        state->window.connect_signal_close_request([](Window*) -> WindowCloseRequestResult {
            std::cout << "close" << std::endl;
            return PREVENT_CLOSE;
        });

        state->window.set_child(&button);
        state->window.present();
    });

    app.connect_signal_shutdown([](Application* app) -> void{
        delete state;
    });

    return app.run();
}