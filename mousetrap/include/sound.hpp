//
// Copyright (c) Clemens Cords (mail@clemens-cords.com), created 3/16/23
//

#pragma once

#include <include/sound_buffer.hpp>
#include <include/time.hpp>
#include <include/vector.hpp>

#include <SFML/Audio/Sound.hpp>

namespace mousetrap
{
    class Sound
    {
        public:
            Sound();

            void create_from_buffer(const SoundBuffer&);

            void play();
            void pause();
            void stop();

            bool get_should_loop() const;
            void set_should_loop(bool);

            Time get_duration() const;

            /// @param fraction: float in [0, 1]
            void set_playback_position(float fraction);

            /// @returns float in [0, 1]
            float get_playback_position() const;

            void set_pitch(float);
            float get_pitch() const;

            void set_volume(float);
            float get_volume() const;

            void set_spacial_position(float x = 0, float y = 0, float z = 0);
            Vector3f get_spacial_position() const;

        private:
            sf::Sound _sound;
    };
}
