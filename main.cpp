#include <mousetrap.hpp>

#include <deque>
#include <iostream>

using namespace mousetrap;

#include <include/g_object_attachment.hpp>

inline struct State {
    Window window;
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

#define G_TYPE_WIDGET_WRAPPER G_TYPE_RIGHT(widget_wrapper)
G_NEW_TYPE(WidgetWrapper, widget_wrapper, WIDGET_WRAPPER, Widget*);

#define G_TYPE_BUTTON_WRAPPER G_TYPE_RIGHT(button_wrapper)
G_NEW_TYPE(ButtonWrapper, button_wrapper, BUTTON_WRAPPER, Button*);

#define NEW(init, owner) detail::attach_ref_to_object(owner.operator GObject*(), wrap(new init))

int main()
{
    auto app = Application("mousetrap.debug");
    std::reference_wrapper<Action>* ref = nullptr;

    app.connect_signal_activate([&](Application* app) -> void
    {
        log::set_surpress_debug(MOUSETRAP_DOMAIN, false);

        state = new State {
    Window(*app)
        };

        auto* button = detail::attach_ref_to_object(state->window.operator GObject*(), wrap(new Button()));
        state->window.set_child(button);
        state->window.present();
    });

    app.connect_signal_shutdown([](Application* app) -> void{
        delete state;
    });

    return app.run();
}