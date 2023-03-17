//
// Created by clem on 3/17/23.
//

#include <include/sound_stream.hpp>
#include <iostream>

#include <cmath>

namespace mousetrap
{
    static const float PI = 3.1415926535f;
    static float seconds_offset = 0.0f;
    static void write_callback(struct SoundIoOutStream *outstream,
    int frame_count_min, int frame_count_max)
    {
        const struct SoundIoChannelLayout *layout = &outstream->layout;
        float float_sample_rate = outstream->sample_rate;
        float seconds_per_frame = 1.0f / float_sample_rate;
        struct SoundIoChannelArea *areas;
        int frames_left = frame_count_max;
        int err;

        while (frames_left > 0) {
            int frame_count = frames_left;

            if ((err = soundio_outstream_begin_write(outstream, &areas, &frame_count))) {
                fprintf(stderr, "%s\n", soundio_strerror(err));
                exit(1);
            }

            if (!frame_count)
                break;

            float pitch = 440.0f;

            float radians_per_second = pitch * 2.0f * PI;
            for (int frame = 0; frame < frame_count; frame += 1) {
                float sample = sinf((seconds_offset + frame * seconds_per_frame) * radians_per_second);
                for (int channel = 0; channel < layout->channel_count; channel += 1) {
                    float *ptr = (float*)(areas[channel].ptr + areas[channel].step * frame);
                    *ptr = sample;
                }
            }
            seconds_offset = fmodf(seconds_offset + seconds_per_frame * frame_count, 1.0f);

            if ((err = soundio_outstream_end_write(outstream))) {
                fprintf(stderr, "%s\n", soundio_strerror(err));
                exit(1);
            }

            frames_left -= frame_count;
        }
    }

    SoundStream::SoundStream()
    {
        if (_soundio == nullptr)
            _soundio = soundio_create();

        if (_soundio == nullptr)
            std::cerr << "[ERROR] In SoundStream::SoundStream: Unable to create soundio" << std::endl;

        int error = soundio_connect(_soundio);
        if (error != 0)
            std::cerr << "[ERROR] In SoundStream::SoundStream: Unable to connect soundio " << soundio_strerror(error) << std::endl;

        soundio_flush_events(_soundio);

        _output_device_index = soundio_default_output_device_index(_soundio);
        if (_output_device_index < 0)
            std::cerr << "[ERROR] In SoundStream::SoundStream: Unable to detect default output device" << std::endl;

        if (_output_device == nullptr)
            _output_device = soundio_get_output_device(_soundio, _output_device_index);

        if (_output_device == nullptr)
            std::cerr << "[ERROR] In SoundStream::SoundStream: Unable to create output device" << std::endl;
    }


    void SoundStream::open_callback(SoundStream* instance)
    {
        instance->_is_open = true;

        int error = soundio_outstream_start(instance->_outstream);
        if (error != 0)
            std::cerr << "[ERROR] In SoundStream::_open_callback: " << soundio_strerror(error) << std::endl;

        soundio_wait_events(instance->_soundio);
    }

    void SoundStream:: open()
    {
        if (_is_open)
            return;

        _outstream = soundio_outstream_create(_output_device);
        _outstream->format = SoundIoFormatFloat32LE;
        _outstream->write_callback = write_callback;

        int error = soundio_outstream_open(_outstream);
        if (error != 0 or _outstream->layout_error)
            std::cerr << "[ERROR] In SoundStream::open: " << soundio_strerror(error) << std::endl;

        _thread = std::jthread(open_callback, this);
    }

    void SoundStream::close()
    {
        if (not _is_open)
            return;

        soundio_outstream_destroy(_outstream);
        _outstream = nullptr;

        _thread.request_stop();
        _thread.join();

        _is_open = false;
    }
};