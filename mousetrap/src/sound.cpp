//
// Created by clem on 3/16/23.
//

#include <include/sound.hpp>

#include <iostream>

namespace mousetrap
{
    Sound::Sound()
    {}

    void Sound::create_from_buffer(const SoundBuffer& buffer)
    {
        auto* sf_buffer = buffer.operator sf::SoundBuffer*();
        if (sf_buffer == nullptr)
        {
            std::cerr << "[WARNING] In Sound::create_from_buffer: Buffer uninitialized" << std::endl;
            return;
        }

        _sound.setBuffer(*buffer.operator sf::SoundBuffer*());
    }

    void Sound::play()
    {
        _sound.play();
    }

    void Sound::pause()
    {
        _sound.pause();
    }

    void Sound::stop()
    {
        _sound.stop();
    }

    bool Sound::get_should_loop() const
    {
        return _sound.getLoop();
    }

    void Sound::set_should_loop(bool b)
    {
        _sound.setLoop(b);
    }

    Time Sound::get_duration() const
    {
        auto duration = _sound.getBuffer()->getDuration();
        return microseconds(duration.asMicroseconds());
    }

    void Sound::set_playback_position(float fraction)
    {
        fraction = glm::clamp<float>(fraction, 0, 1);
        _sound.setPlayingOffset(sf::microseconds(fraction * get_duration().as_microseconds()));
    }

    float Sound::get_playback_position() const
    {
        auto offset = _sound.getPlayingOffset().asMicroseconds();
        return offset / get_duration().as_microseconds();
    }

    void Sound::set_pitch(float pitch)
    {
        _sound.setPitch(pitch);
    }

    float Sound::get_pitch() const
    {
        return _sound.getPitch();
    }

    void Sound::set_volume(float volume)
    {
        _sound.setVolume(volume);
    }

    float Sound::get_volume() const
    {
        return _sound.getVolume();
    }

    void Sound::set_spacial_position(float x, float y, float z)
    {
        if (_sound.getBuffer()->getChannelCount() != 1)
            std::cerr << "[WARNING] In Sound::set_spacial_position: Only 1-Channel (Mono) sounds can be spatialized" << std::endl;

        _sound.setPosition(x, y, z);
    }

    Vector3f Sound::get_spacial_position() const
    {
        auto vec = _sound.getPosition();
        return Vector3f(vec.x, vec.y, vec.z);
    }
}