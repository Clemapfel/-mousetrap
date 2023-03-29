template<typename T>
class has_file_changed_signal
{
    private:
        T* _instance = nullptr;
        std::function<void(T*, FileMonitorEvent, const FileDescriptor& self, const FileDescriptor& other)> _function;
        bool _blocked = false;

        static void wrapper(void*, GFile* self, GFile* other, FileMonitorEvent event, has_file_changed_signal<T>* instance)
        {
            auto self_descriptor = FileDescriptor(self);
            auto other_descriptor = FileDescriptor(other);
            instance->emit_signal_file_changed((FileMonitorEvent) event, self_descriptor, other_descriptor);
        }

    protected:
        explicit has_file_changed_signal(T* instance)
        : _instance(instance)
        {}

    public:
        static inline constexpr const char* signal_id = "changed";

        template<typename Function_t, typename Data_t>
        void connect_signal_file_changed(Function_t function, Data_t data)
        {
            _function = [f = function, d = data](T* instance, FileMonitorEvent event, const FileDescriptor& file, const FileDescriptor& other) -> void
            {
                return f(instance, event, file, other, d);
            };

            static_cast<SignalEmitter*>(_instance)->connect_signal(signal_id, wrapper, this);
        }

        template<typename Function_t>
        void connect_signal_file_changed(Function_t function)
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