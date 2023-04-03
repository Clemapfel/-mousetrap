#include <mousetrap.hpp>

#include <deque>
#include <iostream>

using namespace mousetrap;

inline struct State {
    Window window;
    Button button;
}* state = nullptr;

#include <include/g_object_attachment.hpp>

int main()
{
    auto app = Application("mousetrap.debug");

    app.connect_signal_activate([](Application* app) -> void
    {
        state = new State {
    Window(*app)
        };

        using namespace detail;
        static auto* test_item = detail::wrapper_new(new detail::Test(13));
        detail::attach_ref_to_object(state->window.operator GObject*(), G_OBJECT(test_item));

        state->window.set_child(&state->button);
        state->window.present();
    });

    app.connect_signal_shutdown([](Application* app) -> void{
        delete state;
    });

    return app.run();
}