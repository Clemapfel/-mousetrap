#include <mousetrap.hpp>

#include <deque>
#include <iostream>

using namespace mousetrap;

inline struct State {
    Window window;
    Button button;
}* state = nullptr;

#include <include/g_object_attachment.hpp>

static void toggle_notify(gpointer data, GObject* object, gboolean last_ref)
{
    if (last_ref)
        g_object_unref(data);
}

int main()
{
    auto app = Application("mousetrap.debug");

    app.connect_signal_activate([](Application* app) -> void
    {
        state = new State {
    Window(*app)
        };

        using namespace detail;
        static auto* list = g_object_ref(g_list_store_new(G_TYPE_OBJECT));
        static auto* test_item = detail::wrapper_new(new detail::Test(13));
        g_object_add_toggle_ref(G_OBJECT(list), toggle_notify, test_item);
        g_object_unref(list);

        state->button.connect_signal_clicked([&](Button*){
            g_object_unref(list);
        });
        state->button.set_expand(true);

        state->window.set_child(&state->button);
        state->window.present();
    });

    app.connect_signal_shutdown([](Application* app) -> void{
        delete state;
    });

    return app.run();
}