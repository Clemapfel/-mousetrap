#include <mousetrap.hpp>

#include <deque>
#include <iostream>
#include <thread>

using namespace mousetrap;

#include <include/g_object_attachment.hpp>

inline struct State {
    Window window;
    Window window_01;
}* state = nullptr;


struct Test
{
    size_t _state;

    Test(size_t x)
        : _state(x)
    {}

    ~Test()
    {
        std::cout << "delete " << _state << std::endl;
    }
};

#define G_TYPE_BUTTON_WRAPPER G_TYPE_RIGHT(button_wrapper)
G_NEW_TYPE(ButtonWrapper, button_wrapper, BUTTON_WRAPPER, Button);

int main()
{
    auto app = Application("mousetrap.debug");
    std::reference_wrapper<Action>* ref = nullptr;

    app.connect_signal_activate([](Application* app) -> void
    {
        log::set_surpress_debug(MOUSETRAP_DOMAIN, false);

        state = new State {
    Window(*app),
            Window(*app)
        };

        auto* wrapper = button_wrapper_new();
        auto* button = detail::attach_ref_to_object(state->window.operator GObject*(), wrapper);
        ///detail::attach_ref_to_object(app->operator GObject *(), wrapper);

        button->connect_signal_clicked([](Button* instance){
            std::cout << instance->get_has_frame() << std::endl;
        });

        state->window.set_child(nullptr);
        state->window.present();
        state->window_01.present();
    });

    app.connect_signal_shutdown([](Application* app) -> void{
        delete state;
    });

    return app.run();
}