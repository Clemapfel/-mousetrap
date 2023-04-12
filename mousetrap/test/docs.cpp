//
// Created by clem on 4/12/23.
//

#include <mousetrap.hpp>

using namespace mousetrap;

inline struct State
{
    Window main_window;

    Stack stack;

    StackSidebar stack_control = StackSidebar(stack);
    ScrolledWindow stack_control_window;
    Revealer stack_control_revealer;
    Box stack_control_revealer_wrapper = Box(Orientation::VERTICAL);

    Box stack_box = Box(Orientation::HORIZONTAL);

}* state = nullptr;

int main()
{
    auto app = Application("docs.screenshots");
    app.connect_signal_activate([](Application* app)
    {
        state = new State{
            Window(*app)
        };

        state->stack.add_child(Label("label"), "0");

        // action to hide switcher

        auto hide_stack_control_action = Action("main.show_hide_stack_control");
        hide_stack_control_action.set_function([](){
            auto current = state->stack_control_revealer.get_revealed();
            state->stack_control_revealer.set_revealed(not current);
        });
        hide_stack_control_action.add_shortcut("<Control>h");
        app->add_action(hide_stack_control_action);

        auto shortcut_controller = ShortcutController(app);
        shortcut_controller.add_action(hide_stack_control_action);
        state->main_window.add_controller(shortcut_controller);
        state->stack_control_revealer.set_tooltip_text("<b>Control + H</b>: Hide Selector");

        // layout

        state->stack.set_expand(true);
        state->stack_control_window.set_expand(false);
        state->stack_control_window.set_propagate_natural_width(true);
        state->stack_control_revealer.set_transition_type(RevealerTransitionType::SLIDE_RIGHT);

        state->stack_box.push_back(state->stack);
        state->stack_control_window.set_child(state->stack_control);
        state->stack_control_revealer.set_child(state->stack_control_window);
        state->stack_control_revealer_wrapper.push_back(state->stack_control_revealer);
            // we need to put the revealer inside it's own box to allow the layout manager to reallocate the outer box,
            // properly hiding the revealer by expanding the outer box, as opposed to the revealer simply becoming invsible
            // with empty space left where it has space allocated

        state->stack_box.push_back(state->stack_control_revealer_wrapper);

        state->main_window.set_title("mousetrap");
        state->main_window.set_child(state->stack_box);

        // start main

        state->main_window.present();
    });

    app.connect_signal_shutdown([](Application*){
        delete state;
    });

    return app.run();
}
