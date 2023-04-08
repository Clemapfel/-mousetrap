#include <mousetrap.hpp>

#include <deque>
#include <iostream>
#include <thread>
#include <memory>

using namespace mousetrap;

G_DECLARE_FINAL_TYPE (Super, super, G, SUPER, GtkWidget)

class _Super
{
    private:
        GObject parent_instance;
        size_t member;

    public:
        _Super(size_t x)
            : member(x)
        {}

        ~_Super()
        {
            std::cout << "deleted" << std::endl;
        }
};

class _SuperClass
{
    GObjectClass parent_class;
};

G_DEFINE_TYPE (Super, super, G_TYPE_OBJECT)

static void super_finalize(GObject* object)
{
    auto* self = G_SUPER(object);
    G_OBJECT_CLASS(super_parent_class)->finalize(object);
};

static void super_init(Super*) {}

static void super_class_init(SuperClass* c)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS(c);
    gobject_class->finalize = super_finalize;
}

static Super super_new()
{
    return *((Super*) g_object_new(super_get_type(), nullptr));
}

struct Test
{
    ~Test()
    {
        std::cout << "delete" << std::endl;
    }
};

///
inline struct State {
    Window window;
}* state = nullptr;

int main()
{
    auto app = Application("mousetrap.debug");

    app.connect_signal_activate([&](Application* app) -> void
    {
        log::set_surpress_debug(MOUSETRAP_DOMAIN, false);

        state = new State {
            Window(*app)
        };

        auto action = Action("test.test_action");
        action.set_function([](){
            std::cout << "test" << std::endl;
        });
        action.add_shortcut("<Control>c");
        app->add_action(action);

        static auto shortcut_controller = ShortcutController(app);
        shortcut_controller.add_action(action);
        state->window.add_controller(&shortcut_controller);

        static auto button = Button();
        button.set_action(action);

        state->window.set_child(&button);
        state->window.present();
    });

    app.connect_signal_shutdown([](Application* app) -> void{
        delete state;
    });

    return app.run();
}