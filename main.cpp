#include <mousetrap.hpp>

#include <deque>
#include <iostream>

using namespace mousetrap;

#include <include/g_object_attachment.hpp>

inline struct State {
    Window window;
}* state = nullptr;


struct Test
{
    Test()
        : internal(new TestInternal{14})
    {}

    struct TestInternal
    {
        size_t state;
        ~TestInternal()
        {
            std::cout << "internal deleted" << std::endl;
        }

    }* internal = nullptr;

    ~Test()
    {
        std::cout << "outer deleted" << std::endl;
    }
};

using Internal = Test::TestInternal;
#define G_TYPE_INTERNAL_WRAPPER G_WRAPPER_TYPE_RIGHT(internal)
G_NEW_WRAPPER(Internal, internal, INTERNAL);

int main()
{
    auto app = Application("mousetrap.debug");
    std::reference_wrapper<Action>* ref = nullptr;

    app.connect_signal_activate([&](Application* app) -> void
    {
        log::set_surpress_debug(MOUSETRAP_DOMAIN, false);

        state = new State {
    Window(*app)
        };

        auto test = Test();

        static auto button = Button();
        button.connect_signal_clicked([](Button*, Test::TestInternal* test_internal){
            std::cout << test_internal->state << std::endl;
        }, test.internal);

        detail::attach_ref_to_object(button.operator GObject *(), wrap(test.internal));

        state->window.set_child(&button);
        state->window.present();
    });

    app.connect_signal_shutdown([](Application* app) -> void{
        delete state;
    });

    return app.run();
}