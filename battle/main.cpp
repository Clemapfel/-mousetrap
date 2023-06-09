#include <sol2/include/sol/sol.hpp>
#include <lua.hpp>

#include <include/application.hpp>

using namespace mousetrap;

struct State {
    Window window;
}* state = nullptr;

const std::string RESOURCE_PATH = "/home/clem/Workspace/rat_game/battle/";

int main()
{
    static sol::state lua;
    lua.open_libraries(
        sol::lib::base,
        sol::lib::package,
        sol::lib::os,
        sol::lib::io,
        sol::lib::string,
        sol::lib::table,
        sol::lib::math,
        sol::lib::coroutine
    );

    lua["RESOURCE_PATH"] = RESOURCE_PATH;
    lua.safe_script_file(RESOURCE_PATH + "include.lua");

    return 0;

    auto app = Application("rat_game.battle");
    app.connect_signal_activate([](Application* app)
    {
        state = new State{
            Window(*app)
        };

        state->window.present();
    });

    return app.run();
}