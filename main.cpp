#include <mousetrap.hpp>

#include <deque>
#include <iostream>

using namespace mousetrap;

#include <include/g_object_attachment.hpp>

using VoidFunction = std::function<void(Button*)>;
#define G_TYPE_VOID_FUNCTION_WRAPPER (void_function_wrapper_get_type())
G_NEW_WRAPPER(VoidFunction, void_function, VOID_FUNCTION)

struct ButtonCallback : public Widget
{
    Box _box = Box(Orientation::HORIZONTAL);
    Button _button;

    std::function<void(Button*)>* _callback = nullptr;

    ButtonCallback()
    {
        _box.push_back(&_button);
    }

    operator NativeWidget() const override
    {
        return _box;
    }

    template<typename Function_t>
    void register_callback(Function_t f)
    {
        _callback = new std::function<void(Button*)>(f);
        _button.connect_signal_clicked([](Button* button, std::function<void(Button*)>* f){
            (*f)(button);
        }, _callback);

        detail::attach_ref_to_object(G_OBJECT(_button.operator GtkWidget *()), wrap(_callback));
    }
};

inline struct State {
    Window window;
}* state = nullptr;

int main()
{
    auto app = Application("mousetrap.debug");

    app.connect_signal_activate([](Application* app) -> void
    {
        log::set_surpress_debug(MOUSETRAP_DOMAIN, false);

        state = new State {
    Window(*app)
        };

        auto callback = ButtonCallback();
        callback.register_callback([](Button* button){
            std::cout << "success" << std::endl;
        });

        state->window.set_child(&callback);
        state->window.present();
    });

    app.connect_signal_shutdown([](Application* app) -> void{
        delete state;
    });

    return app.run();
}