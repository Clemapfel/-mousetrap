#include <mousetrap.hpp>

#include <deque>
#include <iostream>

using namespace mousetrap;

class CompoundWidget : public Widget
{
    private:
        Box _box = Box(Orientation::HORIZONTAL);
        Button _button_01, _button_02, _button_03;

    public:
        CompoundWidget()
            : Widget(_box.operator GtkWidget*())
        {
            _button_02.set_child(&_button_03);
            _button_01.set_child(&_button_02);
            _box.push_back(&_button_01);
        }
};

inline struct State {
    Window window;
    CompoundWidget widget;
}* state = nullptr;

int main()
{
    auto app = Application("mousetrap.debug");

    app.connect_signal_activate([](Application* app) -> void
    {
        state = new State {
    Window(*app)
        };

        state->window.set_child(&state->widget);
        //state->window.set_child(&state->window);
        state->window.present();
    });

    app.connect_signal_shutdown([](Application* app) -> void{
        delete state;
    });

    return app.run();
}