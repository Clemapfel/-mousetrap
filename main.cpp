#include <mousetrap.hpp>

#include <deque>
#include <iostream>

using namespace mousetrap;

inline struct State {
    Window window;

}* state = nullptr;

int main()
{
    auto app = Application("mousetrap.debug");

    app.connect_signal_activate([](Application* app) -> void
    {
        state = new State {
        Window(*app)
        };

        state->window.present();
    });

    app.connect_signal_shutdown([](Application* app) -> void{
        delete state;
    });

    return app.run();
}