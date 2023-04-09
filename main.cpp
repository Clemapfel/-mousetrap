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
        button.connect_signal_clicked([](Button*){
           std::cout << "clicked" << std::endl;
        });

        state->window.set_child(&button);
        state->window.present();
    });

    app.connect_signal_shutdown([](Application* app) -> void{
        delete state;
    });

    return app.run();
}