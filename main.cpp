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

        auto spin = SpinButton(0, 1, 0.001);
        spin.set_value_to_text_function([](const SpinButton* instance, float value) -> std::string{
            return std::to_string(value) + "%";
        });

        auto scale = Scale(0, 1, 0.01);

        auto grid = GridView();
        grid.push_back(spin);
        grid.push_back(scale);

        state->window.set_child(grid);
        state->window.present();
    });

    app.connect_signal_shutdown([](Application* app) -> void{
        delete state;
    });

    return app.run();
}