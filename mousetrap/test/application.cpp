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

}* state = nullptr;

int main()
{
    Test::initialize();

    auto app = Application("mousetrap.test.application");

    static bool activate_triggered = false;
    static bool startup_triggered = false;
    static bool shutdown_triggered = false;

    BEGIN_TEST("init")

    Test::assert_that(G_IS_APPLICATION(app.operator GObject*()));
    Test::assert_that(GTK_IS_APPLICATION(app.operator GObject*()));

    END_TEST

    app.connect_signal_activate([&](Application*){

        activate_triggered = true;

        state = new State{
            Window(app)
        };

        BEGIN_TEST("window")
        Test::assert_that(GTK_IS_WINDOW(state->window.operator GtkWidget*()));
        END_TEST

        state->window.show();
        state->window.present();
        state->window.close();
    });

    app.connect_signal_startup([](Application*){
        startup_triggered = true;
    });

    app.connect_signal_shutdown([](Application*){
        shutdown_triggered = true;
        delete state;
    });

    auto res = app.run();

    BEGIN_TEST("signals")

    Test::assert_that(activate_triggered);
    Test::assert_that(startup_triggered);
    Test::assert_that(shutdown_triggered);

    END_TEST

    return Test::conclude();
}