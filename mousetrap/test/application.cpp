//
// Created by clem on 3/28/23.
//

#include "test.hpp"

#include <include/application.hpp>
#include <include/window.hpp>
#include <include/toggle_button.hpp>
#include <include/label.hpp>

using namespace mousetrap;

inline struct State
{
    Application app;

    ToggleButton _hold_toggle_button;


}* state = nullptr;

int main()
{
    Test::initialize();

    state = new State{
    Application("mousetrap.test.application"),
    };

    static bool activate_triggered = false;
    static bool startup_triggered = false;
    static bool shutdown_triggered = false;

    BEGIN_TEST("init")

    Test::assert_that(G_IS_APPLICATION(state->app.operator GObject*()));
    Test::assert_that(GTK_IS_APPLICATION(state->app.operator GObject*()));

    CONTINUE_TEST("signals")

    state->app.connect_signal_activate([](Application*){
        activate_triggered = true;
    });

    state->app.connect_signal_startup([](Application*){
        startup_triggered = true;
    });

    state->app.connect_signal_shutdown([](Application*){
        shutdown_triggered = true;
    });

    CONTINUE_TEST("run")

    auto res = state->app.run();

    Test::assert_that(activate_triggered);
    Test::assert_that(startup_triggered);
    Test::assert_that(shutdown_triggered);

    END_TEST

    delete state;
    return Test::conclude();
}