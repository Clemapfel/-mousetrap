//
// Created by clem on 3/28/23.
//

#include "test.hpp"

#include <include/application.hpp>
#include <include/window.hpp>
#include <include/toggle_button.hpp>
#include <include/box.hpp>
#include <include/label.hpp>

using namespace mousetrap;

inline struct State
{
    Window window;

    Label instruction_label = Label("Press Ctrl+C to exit");

    Action stateless_action = Action("test.statelesss");
    Action stateful_action = Action("test.stateful");
    Action close_action = Action("test.close");

    MenuModel menubar_menu_model = MenuModel();
    
}* state = nullptr;

static bool stateless_triggered = false;
static bool stateful_triggered = false;

int main()
{
    Test::initialize();
    auto app = Application("mousetrap.test.application");

    app.connect_signal_activate([](Application* app){

        state = new State{
            Window(*app)
        };

        state->window.set_is_decorated(false);
        state->instruction_label.set_margin(20);

        BEGIN_TEST("actions uninitialized")
            Test::assert_that(not G_IS_ACTION(state->close_action.operator GAction*()));
            Test::assert_that(not G_IS_ACTION(state->stateless_action.operator GAction*()));
            Test::assert_that(not G_IS_ACTION(state->stateful_action.operator GAction*()));
        END_TEST

        state->stateful_action.set_stateful_function([](bool in) -> bool {
            stateful_triggered = true;
            return not in;
        });

        state->stateless_action.set_function([]() {
            stateless_triggered = true;
        });

        state->close_action.set_function([](){
            state->window.close();
        });
        state->close_action.add_shortcut("<Control>c");

        app->add_action(&state->close_action);
        app->add_action(&state->stateless_action);
        app->add_action(&state->stateful_action);

        auto submenu = MenuModel();
        submenu.add_action("Stateless Action", state->stateless_action);
        submenu.add_stateful_action("Stateful Action", state->stateful_action, true);
        submenu.add_action("Close", state->close_action);

        BEGIN_TEST("actions initialized")
            Test::assert_that(G_IS_ACTION(state->close_action.operator GAction*()));
            Test::assert_that(G_IS_ACTION(state->stateless_action.operator GAction*()));
            Test::assert_that(G_IS_ACTION(state->stateful_action.operator GAction*()));
        END_TEST

        state->menubar_menu_model.add_submenu("Test", &submenu);
        app->set_menubar(&state->menubar_menu_model);

        state->stateful_action.activate();
        state->stateless_action.activate();

        state->window.set_show_menubar(true);
        state->window.set_child(&state->instruction_label);
        state->window.present();
    });

    app.connect_signal_shutdown([](Application*){

        BEGIN_TEST("actions triggered")

        Test::assert_that(stateless_triggered);
        Test::assert_that(stateful_triggered);

        END_TEST

        delete state;
    });

    auto res = app.run();
    return Test::conclude();
}