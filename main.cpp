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
        seconds_offset = fmodf(seconds_offset +
                               seconds_per_frame * frame_count, 1.0f);

        if ((err = soundio_outstream_end_write(outstream))) {
            fprintf(stderr, "%s\n", soundio_strerror(err));
            exit(1);
        }

        frames_left -= frame_count;
    }
}

struct SoundStream
{
    SoundIo* _soundio = nullptr;
    int _output_device_index = -1;
    SoundIoDevice* _output_device = nullptr;
    SoundIoOutStream* _outstream = nullptr;

    SoundStream()
    {
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

        _output_device = soundio_get_output_device(_soundio, _output_device_index);
        if (_output_device == nullptr)
            std::cerr << "[ERROR] In SoundStream::SoundStream: Unable to create output device" << std::endl;

        _outstream = soundio_outstream_create(_output_device);
        _outstream->format = std::endian::native == std::endian::little ? SoundIoFormatS16LE : SoundIoFormatS16BE;
        _outstream->write_callback = write_callback;
    }

    ~SoundStream()
    {
        if (_outstream != nullptr)
            soundio_outstream_destroy(_outstream);

        if (_output_device != nullptr)
            soundio_device_unref(_output_device);

        if (_soundio != nullptr)
            soundio_destroy(_soundio);
    }

    void open()
    {
        int error = soundio_outstream_open(_outstream);
        if (error != 0)
            goto error;

        if (_outstream->layout_error)
            goto error;

        error = soundio_outstream_start(_outstream);
        if (error != 0)
            goto error;

        while (true)
            soundio_wait_events(_soundio);

        error:
            std::cout << "[ERROR] In SoundStream::open: " << soundio_strerror(error) << std::endl;
    }
};

int main(int argc, char **argv)
{
    int err;
    struct SoundIo *soundio = soundio_create();
    if (!soundio) {
        fprintf(stderr, "out of memory\n");
        return 1;
    }

    if ((err = soundio_connect(soundio))) {
        fprintf(stderr, "error connecting: %s", soundio_strerror(err));
        return 1;
    }

    soundio_flush_events(soundio);

    int default_out_device_index = soundio_default_output_device_index(soundio);
    if (default_out_device_index < 0) {
        fprintf(stderr, "no output device found");
        return 1;
    }

    struct SoundIoDevice *device = soundio_get_output_device(soundio, default_out_device_index);
    if (!device) {
        fprintf(stderr, "out of memory");
        return 1;
    }

    fprintf(stderr, "Output device: %s\n", device->name);

    struct SoundIoOutStream *outstream = soundio_outstream_create(device);
    outstream->format = SoundIoFormatFloat32NE;
    outstream->write_callback = write_callback;

    //
    if ((err = soundio_outstream_open(outstream))) {
        fprintf(stderr, "unable to open device: %s", soundio_strerror(err));
        return 1;
    }

    if (outstream->layout_error)
        fprintf(stderr, "unable to set channel layout: %s\n", soundio_strerror(outstream->layout_error));

    if ((err = soundio_outstream_start(outstream))) {
        fprintf(stderr, "unable to start device: %s", soundio_strerror(err));
        return 1;
    }

    for (;;)
        soundio_wait_events(soundio);

    soundio_outstream_destroy(outstream);
    soundio_device_unref(device);
    soundio_destroy(soundio);
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