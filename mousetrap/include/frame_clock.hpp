//
// Copyright 2022 Clemens Cords
// Created on 11/11/22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <gtk/gtk.h>
#include <include/time.hpp>
#include <include/signal_emitter.hpp>
#include <include/signal_component.hpp>

namespace mousetrap
{
    class FrameClock : public SignalEmitter,
        HAS_SIGNAL(FrameClock, update),
        HAS_SIGNAL(FrameClock, paint)
    {
        public:
            FrameClock(GdkFrameClock*);
            virtual ~FrameClock();

            FrameClock(const FrameClock&) = delete;
            FrameClock(FrameClock&&) noexcept;

            FrameClock& operator=(const FrameClock&) = delete;
            FrameClock& operator=(FrameClock&&) noexcept;

            operator GObject*();

            Time get_frame_time();
            Time get_time_since_last_frame();

            float get_fps();

            void start();
            void stop();

        private:
            GdkFrameClock* _native = nullptr;
    };
}
