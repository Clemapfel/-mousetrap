//
// Copyright (c) Clemens Cords (mail@clemens-cords.com), created 4/13/23
//

#pragma once

#include <mousetrap.hpp>
#include "test_component.hpp"

using namespace mousetrap;

class SignalsChapter : public TestComponent
{
    private:
        Button button;

    public:
        std::string get_title() const override {
            return "Chapter 3: Button HelloWorld";
        }

        operator NativeWidget() const override {
            return button;
        }

        SignalsChapter()
        {
            button.connect_signal_clicked([](Button* button, std::string data){
                std::cout << data << std::endl;
            }, "clicked");
        }
};
