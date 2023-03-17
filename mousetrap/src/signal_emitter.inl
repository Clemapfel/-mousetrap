//
// Copyright (c) Clemens Cords (mail@clemens-cords.com), created 3/17/23
//

namespace mousetrap
{
    template<typename Function_t>
    void SignalEmitter::connect_signal(const std::string& signal_id, Function_t* function, void* data)
    {
        auto handler_id = g_signal_connect(operator GObject*(), signal_id.c_str(), G_CALLBACK(function), data);
        _signal_handlers.insert_or_assign(signal_id, SignalHandler{handler_id});
    }
}