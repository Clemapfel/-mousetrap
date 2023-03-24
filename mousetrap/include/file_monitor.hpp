//
// Copyright (c) Clemens Cords (mail@clemens-cords.com), created 3/23/23
//

#pragma once

#include <include/signal_emitter.hpp>
#include <include/signal_component.hpp>

#include <functional>

namespace mousetrap
{
    enum class FileMonitorEvent
    {
        CHANGED = G_FILE_MONITOR_EVENT_CHANGED,
        CHANGES_DONE_HINT = G_FILE_MONITOR_EVENT_CHANGES_DONE_HINT,
        DELETED = G_FILE_MONITOR_EVENT_DELETED,
        CREATED = G_FILE_MONITOR_EVENT_CREATED,
        ATTRIBUTE_CHANGED = G_FILE_MONITOR_EVENT_ATTRIBUTE_CHANGED,
        RENAMED = G_FILE_MONITOR_EVENT_RENAMED,
        MOVED_IN = G_FILE_MONITOR_EVENT_MOVED_IN,
        MOVED_OUT = G_FILE_MONITOR_EVENT_MOVED_OUT
    };

    class FileMonitor : public SignalEmitter
    {
        friend class FileDescriptor;

        public:
            /// @brief default ctor deleted, use FileDescriptor::create_monitor
            FileMonitor() = delete;
            ~FileMonitor();

            FileMonitor(const FileMonitor&) = delete;
            FileMonitor(FileMonitor&&) noexcept;
            FileMonitor& operator=(const FileMonitor&) = delete;
            FileMonitor& operator=(FileMonitor&&) noexcept;

            operator GObject*() override;

            void cancel();
            bool is_cancelled() const;

            // TODO

        protected:
            FileMonitor(GFileMonitor*);

        private:
            GFileMonitor* _native;
            static void on_changed(GFileMonitor*, GFile*, GFile*, GFileMonitorEvent, FileMonitor* instance);
            static inline gint flags = G_FILE_MONITOR_NONE | G_FILE_MONITOR_WATCH_MOVES;
    };
}
