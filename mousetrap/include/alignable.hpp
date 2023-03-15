//
// Copyright (c) Clemens Cords (mail@clemens-cords.com), created 3/15/23
//

#pragma once

namespace mousetrap
{
    enum class Alignment
    {
        START,
        CENTER,
        END
    };

    class Alignable
    {
        public:
            void set_horizontal_alignment(Alignment);
            void set_vertical_alignment(Alignment);
            void set_alignment(Alignment);

            Alignment get_horizontal_alignment() const;
            Alignment get_vertical_alignment() const;

        private:
            Alignment _horizontal_alignment = Alignment::CENTER;
            Alignment _vertical_alignment = Alignment::CENTER;
    };
}
