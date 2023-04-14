#include <mousetrap.hpp>

#include <deque>
#include <iostream>
#include <thread>
#include <memory>

using namespace mousetrap;

template<typename T>
struct Super
{
    struct Internal
    {
        Internal(T* in)
            : hold(std::make_shared<T*>(in))
        {
            std::cout << "internal ctor: " << hold.get() << std::endl;
        }

        ~Internal()
        {
            std::cout << "internal dtor: " << hold.get() << std::endl;
        }
        std::shared_ptr<T*> hold;
    };

    Internal* _internal;

    Super(T* in)
    {
        _internal = new Internal(in);
    }

    ~Super()
    {
        std::cout << "supet dtor: " << _internal->hold.get() << std::endl;
    }
};

struct Test : public Super<Test>
{
    Test(size_t n)
        : Super<Test>(this), n(n)
    {}

    ~Test()
    {
        std::cout << "test dtor: " << n << std::endl;
    }

    size_t n;
};

int main()
{
    Test* test = nullptr;
    Super<Test>::Internal* internal = nullptr;
    {
        test = new Test(1234);
        internal = test->_internal;
        delete test;
    }

    std::cout << ((Test*) internal->hold.get())->n << std::endl;
    return 0;


    auto app = Application("mousetrap.debug");
    app.connect_signal_activate([&](Application* app) -> void
    {
        auto window = Window(*app);
        auto label = Label("hello world");


        window.set_child(label);
        window.present();
    });

    return app.run();
}