//
// Copyright (c) Clemens Cords (mail@clemens-cords.com), created 3/17/23
//

#pragma once

#include <soundio/soundio.h>
#include <thread>

namespace mousetrap
{
    class SoundStream
    {
        public:
            SoundStream();

            SoundStream(const SoundStream&) = delete;
            SoundStream(SoundStream&&) = delete;
            SoundStream& operator=(const SoundStream&) noexcept = delete;
            SoundStream& operator=(SoundStream&&) noexcept = delete;

            // TODO: how to generate composable signals?

            void open();
            void close();

        private:
            static inline SoundIo* _soundio = nullptr;
            int _output_device_index = -1;
            static inline SoundIoDevice* _output_device = nullptr;

            std::atomic<bool> _is_open = false;
            SoundIoOutStream* _outstream = nullptr;
            std::jthread _thread;

            static void open_callback(SoundStream* instance);
    };
}
