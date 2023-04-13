//
// Copyright (c) Clemens Cords (mail@clemens-cords.com), created 4/12/23
//

#pragma once

#include <mousetrap.hpp>
#include "test_component.hpp"

namespace mousetrap
{
    class LabelTest : public TestComponent
    {
        private:
            Paned left_paned = Paned(Orientation::HORIZONTAL);
            Paned right_paned = Paned(Orientation::HORIZONTAL);

            Box main = Box(Orientation::VERTICAL);

        public:
            LabelTest() {
                initialize();
            }

            std::string get_title() const override {
                return "Label";
            }

            operator NativeWidget() const override {
                return main.operator NativeWidget();
            }

            void initialize()
            {
            }
    };
}
