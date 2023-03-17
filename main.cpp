#include <SFML/Window.hpp>

#include <mousetrap/include/shape.hpp>
#include <mousetrap/include/render_task.hpp>
#include <mousetrap/include/image.hpp>
#include <mousetrap/include/signals.hpp>
#include <mousetrap/include/rectangle_shape.hpp>
#include <mousetrap/include/box.hpp>
#include <mousetrap/include/sound.hpp>

#include <iostream>

#include <soundio/soundio.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

using namespace mousetrap;

#include <soundio/soundio.h>
#include <thread>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <bit>

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

struct SoundStream
{
    static inline SoundIo* _soundio = nullptr;
    int _output_device_index = -1;
    static inline SoundIoDevice* _output_device = nullptr;

    std::atomic<bool> _is_open = false;
    SoundIoOutStream* _outstream = nullptr;
    std::jthread _thread;

    SoundStream()
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

    ~SoundStream()
    {}

    static void _open_callback(SoundStream* instance)
    {
        instance->_is_open = true;

        int error = soundio_outstream_start(instance->_outstream);
        if (error != 0)
            std::cerr << "[ERROR] In SoundStream::_open_callback: " << soundio_strerror(error) << std::endl;

        soundio_wait_events(instance->_soundio);
    }

    void open()
    {
        if (_is_open)
            return;

        _outstream = soundio_outstream_create(_output_device);
        _outstream->format = SoundIoFormatFloat32BE; //std::endian::native == std::endian::little ? SoundIoFormatS16LE : SoundIoFormatS16BE;
        _outstream->write_callback = write_callback;

        int error = soundio_outstream_open(_outstream);
        if (error != 0 or _outstream->layout_error)
            std::cerr << "[ERROR] In SoundStream::open: " << soundio_strerror(error) << std::endl;

        _thread = std::jthread(_open_callback, this);
    }

    void close()
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

int main(int argc, char **argv)
{
    auto stream = SoundStream();

    for (size_t i = 0; i < 2; ++i)
    {
        std::cout << "open" << std::endl;
        stream.open();
        std::this_thread::sleep_for(std::chrono::seconds(2));
        stream.close();
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
    std::cout << "quit" << std::endl;
    return 0;
}

/*

int main()
{
    sf::Window window(sf::VideoMode(800, 600), "OpenGL", sf::Style::Default, sf::ContextSettings(
    0, 0, 8, 3, 2
    ));
    window.setVerticalSyncEnabled(true);
    window.setActive(true);

    initialize_opengl();

    std::vector<Shape> shapes;
    std::vector<RenderTask> tasks;
    const size_t n = 10;
    for (size_t x = 0; x < n; ++x)
    {
        shapes.emplace_back();
        std::cout << (x / float(n+1)) << std::endl;
        shapes.back().as_rectangle({x / float(n), x / float(n)}, {0.05, 0.05});
        shapes.back().set_color(HSVA((x+1) / float(n), 1, 1, 1));
    }

    for (auto& shape : shapes)
        tasks.emplace_back(&shape, nullptr, nullptr, BlendMode::NORMAL);

    auto shader = Shader();
    auto transform = GLTransform();

    std::deque<RectangleShape> shapes;
    for (size_t x = 0; x < 4; ++x)
    {
        shapes.emplace_back(Vector2f{0.25, 0.25}, Vector2f{0.5, 0.5});
        shapes.back().set_color(HSVA(rand() / float(RAND_MAX), 1, 1, 1));
    }

    Box box = Box(Orientation::HORIZONTAL);
    box.set_reformatting_blocked(true);

    std::deque<RenderTask> tasks;
    for (auto& shape : shapes)
    {
        tasks.push_back(RenderTask(shape.get_shape()));
        box.push_back(&shape);
    }

    box.set_centroid({0.5, 0.5});
    box.set_size({1, 1});
    box.set_reformatting_blocked(false);

    bool running = true;
    while (running)
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                running = false;
            else if (event.type == sf::Event::Resized)
                glViewport(0, 0, event.size.width, event.size.height);
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (auto& task : tasks)
            task.render();

        glFlush();
        window.display();
    }

    return 0;
}

*/