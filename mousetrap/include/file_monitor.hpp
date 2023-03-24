//
// Copyright (c) Clemens Cords (mail@clemens-cords.com), created 3/23/23
//

#pragma once

#include <include/signal_emitter.hpp>
#include <include/signal_component.hpp>
#include <include/file_system.hpp>

namespace mousetrap
{
    class FileMonitor : public SignalEmitter,
        HAS_SIGNAL(FileMonitor, changed)
    {
            // TODO
    };
}
