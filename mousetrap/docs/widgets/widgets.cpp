//
// Created by clem on 4/5/23.
//

#include <mousetrap.hpp>

using namespace mousetrap;

inline struct State
{
    Window window;

    Stack stack;
    StackSidebar stack_sidebar = StackSidebar(stack);
    Box stack_box = Box(Orientation::VERTICAL);

}* state = nullptr;

int main()
{
    auto app = Application("mousetrap.docs");

    app.connect_signal_activate([](Application* app){

        log::set_surpress_debug(MOUSETRAP_DOMAIN, false);

        state = new State{
        Window(*app)
        };

        state->stack_box.push_back(&state->stack);
        state->stack_box.push_back(&state->stack_sidebar);
        state->window.set_child(&state->stack_box);
        state->window.set_size_request({500, 500});
        state->window.show();
    });

    app.connect_signal_shutdown([](Application* app){
       delete state;
    });

    return app.run();
}
