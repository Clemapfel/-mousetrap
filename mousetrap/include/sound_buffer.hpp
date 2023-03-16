//
// Copyright (c) Clemens Cords (mail@clemens-cords.com), created 3/16/23
//

#pragma once

#include <SFML/Audio/SoundBuffer.hpp>

namespace mousetrap
{
    class SoundBuffer
    {
        public:
            using Sample_t = int16_t;

            SoundBuffer();
            virtual ~SoundBuffer();

            void create(size_t sample_count, size_t channel_cound, size_t sample_rate);

            bool create_from_file(const std::string&);
            bool save_to_file(const std::string&);

            size_t get_n_samples() const;
            const Sample_t* get_samples() const;

            operator sf::SoundBuffer*() const;

        private:
            sf::SoundBuffer* _data = nullptr;
    };
}
