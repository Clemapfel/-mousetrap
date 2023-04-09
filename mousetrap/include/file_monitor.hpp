//
// Copyright (c) Clemens Cords (mail@clemens-cords.com), created 3/23/23
//

#pragma once

#include <include/signal_emitter.hpp>
#include <include/signal_component.hpp>
#include <include/file_system.hpp>

#include <functional>

#ifdef DOXYGEN
    #include "../docs/doxygen.inl"
#endif

namespace mousetrap
{
    /// @brief hints type of mutation done to file
    enum class FileMonitorEvent
    {
        /// @brief file has changed in any way
        CHANGED = G_FILE_MONITOR_EVENT_CHANGED,

        /// @brief hints that this event is the last in a series of changes
        CHANGES_DONE_HINT = G_FILE_MONITOR_EVENT_CHANGES_DONE_HINT,

        /// @brief file was deleted
        DELETED = G_FILE_MONITOR_EVENT_DELETED,

        /// @brief file was created
        CREATED = G_FILE_MONITOR_EVENT_CREATED,

        /// @brief an attribute such as metadata or file type changed
        ATTRIBUTE_CHANGED = G_FILE_MONITOR_EVENT_ATTRIBUTE_CHANGED,

        /// @brief file was renamed
        RENAMED = G_FILE_MONITOR_EVENT_RENAMED,

        /// @brief if target is a directory, a file was moved into it
        MOVED_IN = G_FILE_MONITOR_EVENT_MOVED_IN,

        /// @brief if target is a directory, a file was moved out of it
        MOVED_OUT = G_FILE_MONITOR_EVENT_MOVED_OUT
    };

    template<typename T>
    class SIGNAL_CLASS_NAME(file_changed)
    {
        private:
            T* _instance = nullptr;
            std::function<void(T*, FileMonitorEvent, const FileDescriptor& self, const FileDescriptor& other)> _function;
            bool _blocked = false;

            static void wrapper(void*, GFile* self, GFile* other, FileMonitorEvent event, SIGNAL_CLASS_NAME(file_changed)<T>* instance)
            {
                auto self_descriptor = FileDescriptor(self);
                auto other_descriptor = FileDescriptor(other);
                instance->emit_signal_file_changed((FileMonitorEvent) event, self_descriptor, other_descriptor);
            }

        protected:
            explicit SIGNAL_CLASS_NAME(file_changed)(T* instance)
                : _instance(instance)
            {}

        public:
            static inline constexpr const char* signal_id = "changed";

            template<typename Data_t>
            using signal_handler_with_data_f = std::function<void(T* instance, FileMonitorEvent event, const FileDescriptor& self, const FileDescriptor& other, Data_t data)>;

            template<typename Data_t>
            void connect_signal_file_changed(const signal_handler_with_data_f<Data_t>& function, Data_t data)
            {
                _function = [f = function, d = data](T* instance, FileMonitorEvent event, const FileDescriptor& file, const FileDescriptor& other) -> void
                {
                    return f(instance, event, file, other, d);
                };

                static_cast<SignalEmitter*>(_instance)->connect_signal(signal_id, wrapper, this);
            }

            using signal_handler_without_data_f = std::function<void(T* instance, FileMonitorEvent event, const FileDescriptor& self, const FileDescriptor& other)>;

            void connect_signal_file_changed(const signal_handler_without_data_f& function)
            {
                _function = [f = function](T* instance, FileMonitorEvent event, const FileDescriptor& file, const FileDescriptor& other) -> void
                {
                    return f(instance, event, file, other);
                };

                static_cast<SignalEmitter*>(_instance)->connect_signal(signal_id, wrapper, this);
            }

            void set_signal_file_changed_blocked(bool b)
            {
                _blocked = b;
            }

            bool get_signal_file_changed_blocked() const
            {
                return _blocked;
            }

            void emit_signal_file_changed(FileMonitorEvent event, const FileDescriptor& file, const FileDescriptor& other)
            {
                if (not _blocked)
                    return _function(_instance, event, file, other);
            }

            void disconnect_signal_file_changed()
            {
                _instance->disconnect_signal(signal_id);
            }
    };

    /// @class has_signal_file_changed
    /// @brief signal emitted when a file on disk changes
    /// @tparam T instance type
    ///
    /// @fn void has_signal_file_changed::emit_signal_file_changed(FileMonitorEvent event, const FileDescriptor& file, const FileDescriptor& other)
    /// \signal_emit_brief
    /// @param event FileMonitorEvent that describes in what way the file changed
    /// @param file file that changed, this is the file that is being monitored
    /// @param other other file, empty unless event is FileMonitorEvent::MOVED_IN or FileMonitorEvent::MOVED_OUT, in which case it decribes the other file that was moved in or out of the monitored directory
    ///
    /// @var has_signal_file_changed::signal_id
    /// \signal_id{https://docs.gtk.org/gio/signal.FileMonitor.changed.html}
    ///
    /// @var has_signal_file_changed::signal_handler_with_data_f
    /// \signal_with_data_f{file_changed}
    /// \signal_see_emit_for_parameters{file_changed}
    ///
    /// @var has_signal_file_changed::signal_handler_without_data_f
    /// \signal_without_data_f{file_changed}
    /// \signal_see_emit_for_parameters{file_changed}
    ///
    /// @fn void has_signal_file_changed::connect_signal_file_changed(const signal_handler_with_data_f<Data_t>& function, Data_t data)
    /// \signal_connect_data
    ///
    /// @fn void has_signal_file_changed::connect_signal_file_changed(const signal_handler_without_data_f&)
    /// \signal_connect_no_data
    ///
    /// @fn void has_signal_file_changed::set_signal_file_changed_blocked(bool)
    /// \signal_set_blocked
    ///
    /// @fn bool has_signal_file_changed::get_signal_file_changed_blocked() const
    /// \signal_get_blocked
    ///
    /// @fn void has_signal_file_changed::disconnect_signal_file_changed()
    /// \signal_disconnect
    ///
    /// @fn has_signal_file_changed::has_signal_file_changed
    /// \signal_ctor

    /// @brief monitors changes to a file that may or may not yet exist
    class FileMonitor :  public SignalEmitter,
        HAS_SIGNAL(FileMonitor, file_changed)
    {
        friend class FileDescriptor;

        public:
            /// @brief default ctor deleted, use FileDescriptor::create_monitor
            FileMonitor() = delete;

            /// @brief default dtor, this removes any connected signals
            ~FileMonitor();

            /// @brief copy ctor deleted, use FileDescriptor::create_monitor to create an additional monitor to the same file
            FileMonitor(const FileMonitor&) = delete;

            /// @brief copy assignment deleted
            FileMonitor& operator=(const FileMonitor&) = delete;

            /// @brief move ctor
            /// @param other
            FileMonitor(FileMonitor&&) noexcept;

            /// @brief move assignment
            /// @param other
            /// @return reference to self after assignment
            FileMonitor& operator=(FileMonitor&&) noexcept;

            /// @brief expose as GObject \internal
            operator GObject*() override;

            /// @brief cancel the file monitor, cannot be undone
            void cancel();

            /// @brief check if file monitor was cancelled
            /// @return true if cancelled, false otherwise
            bool is_cancelled() const;

        protected:
            FileMonitor(GFileMonitor*);

        private:
            GFileMonitor* _native;
            static void on_changed(GFileMonitor*, GFile*, GFile*, GFileMonitorEvent, FileMonitor* instance);
            static inline gint flags = G_FILE_MONITOR_NONE | G_FILE_MONITOR_WATCH_MOVES;
    };
}
