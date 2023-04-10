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

        auto model = MenuModel();

        auto spin_model = MenuModel();
        model.add_widget(spin);
        model.add_submenu("spin", spin_model);

        auto scale_model = MenuModel();
        model.add_widget(scale);
        model.add_submenu("scale", scale_model);

        auto menu_button = PopoverMenuButton();
        auto menu_popover = PopoverMenu(model);
        menu_button.set_popover_menu(menu_popover);

        auto box = Box(Orientation::VERTICAL);
        box.push_back(menu_popover);

        state->window.set_child(box);
        state->window.present();
    });

    app.connect_signal_shutdown([](Application* app) -> void{
        delete state;
    });

    return app.run();
}