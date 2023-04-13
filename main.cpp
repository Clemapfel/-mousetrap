#include <mousetrap.hpp>

#include <deque>
#include <iostream>
#include <thread>
#include <memory>

using namespace mousetrap;

int main()
{
    auto app = Application("mousetrap.debug");
    app.connect_signal_activate([&](Application* app) -> void
    {
        auto window = Window(*app);
        auto label = Label("hello world");
        window.set_child(label);
        window.present();
    });

    return app.run();
}