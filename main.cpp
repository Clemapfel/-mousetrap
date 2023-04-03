#include <mousetrap.hpp>

#include <deque>
#include <iostream>

using namespace mousetrap;

class CompoundWidget : public Widget
{
    private:
        Box _box = Box(Orientation::HORIZONTAL);
        Button _button_01, _button_02, _button_03;

        static void clicked(Button* button, Box* box)
        {
            std::cout << "clicked" << std::endl;
            std::cout << box.get().get_spacing() << std::endl;
        }

    public:
        CompoundWidget()
        {
            _button_02.set_child(&_button_03);
            _button_01.set_child(&_button_02);
            _box.push_back(&_button_01);
            _button_01.connect_signal_clicked(clicked, std::ref(_box));
        }

        operator NativeWidget() const override
        {
            return _box;
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

        auto* widget = new CompoundWidget();
        state->widget = std::move(*widget);

        state->window.set_child(&state->widget);
        state->window.present();
    });

    app.connect_signal_shutdown([](Application* app) -> void{
        delete state;
    });

    return app.run();
}