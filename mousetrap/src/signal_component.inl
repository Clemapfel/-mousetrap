//
// Copyright (c) Clemens Cords (mail@clemens-cords.com), created 3/18/23
//

#include <iostream>

namespace mousetrap
{
    // KEY PRESSED

    template<typename T>
    bool has_key_pressed_signal<T>::wrapper(void*, uint32_t keyval, uint32_t keycode, ModifierState modifier, has_key_pressed_signal<T>* instance)
    {
        if (instance->_blocked)
            return instance->_function(instance->_instance, keyval, keycode, modifier);
        else
            return false;
    }

    template<typename T>
    template<typename Function_t, typename Data_t>
    void has_key_pressed_signal<T>::connect_signal_key_pressed(Function_t function, Data_t data)
    {
        _function = [f = function, d = data](T* instance, uint32_t keyval, uint32_t keycode, ModifierState modifier)
        {
            return f(instance, keyval, keycode, modifier);
        };
        static_cast<SignalEmitter*>(_instance)->connect_signal(signal_id, wrapper, this);
    }

    template<typename T>
    template<typename Function_t>
    void has_key_pressed_signal<T>::connect_signal_key_pressed(Function_t function)
    {
        _function = [f = function](T* instance, uint32_t keyval, uint32_t keycode, ModifierState modifier)
        {
            return f(instance, keyval, keycode, modifier);
        };
        static_cast<SignalEmitter*>(_instance)->connect_signal(signal_id, wrapper, this);
    }

    // KEY RELEASED

    template<typename T>
    void has_key_released_signal<T>::wrapper(void*, uint32_t keyval, uint32_t keycode, ModifierState modifier, has_key_released_signal<T>* instance)
    {
        if (instance->_blocked)
            instance->_function(instance->_instance, keyval, keycode, modifier);
    }

    template<typename T>
    template<typename Function_t, typename Data_t>
    void has_key_released_signal<T>::connect_signal_key_released(Function_t function, Data_t data)
    {
        _function = [f = function, d = data](T* instance, uint32_t keyval, uint32_t keycode, ModifierState modifier)
        {
            return f(instance, keyval, keycode, modifier);
        };
        static_cast<SignalEmitter*>(_instance)->connect_signal(signal_id, wrapper, this);
    }

    template<typename T>
    template<typename Function_t>
    void has_key_released_signal<T>::connect_signal_key_released(Function_t function)
    {
        _function = [f = function](T* instance, uint32_t keyval, uint32_t keycode, ModifierState modifier)
        {
            return f(instance, keyval, keycode, modifier);
        };
        static_cast<SignalEmitter*>(_instance)->connect_signal(signal_id, wrapper, this);
    }

    // MODIFIERS CHANGED

    template<typename T>
    bool has_modifiers_changed_signal<T>::wrapper(void*, uint32_t keyval, uint32_t keycode, ModifierState modifier, has_modifiers_changed_signal<T>* instance)
    {
        if (instance->_blocked)
            return instance->_function(instance->_instance, keyval, keycode, modifier);
        else
            return false;
    }

    template<typename T>
    template<typename Function_t, typename Data_t>
    void has_modifiers_changed_signal<T>::connect_signal_modifiers_changed(Function_t function, Data_t data)
    {
        _function = [f = function, d = data](T* instance, uint32_t keyval, uint32_t keycode, ModifierState modifier)
        {
            return f(instance, keyval, keycode, modifier);
        };
        static_cast<SignalEmitter*>(_instance)->connect_signal(signal_id, wrapper, this);
    }

    template<typename T>
    template<typename Function_t>
    void has_modifiers_changed_signal<T>::connect_signal_modifiers_changed(Function_t function)
    {
        _function = [f = function](T* instance, uint32_t keyval, uint32_t keycode, ModifierState modifier)
        {
            return f(instance, keyval, keycode, modifier);
        };
        static_cast<SignalEmitter*>(_instance)->connect_signal(signal_id, wrapper, this);
    }

    // MOTION ENTER

    template<typename T>
    void has_motion_enter_signal<T>::wrapper(void*, float x, float y, has_motion_enter_signal<T>* instance)
    {
        if (instance->_blocked)
            instance->_function(instance->_instance, x, y);
    }

    template<typename T>
    template<typename Function_t, typename Data_t>
    void has_motion_enter_signal<T>::connect_signal_motion_enter(Function_t function, Data_t data)
    {
        _function = [f = function, d = data](T* instance, float x, float y)
        {
            return f(instance, x, y);
        };
        static_cast<SignalEmitter*>(_instance)->connect_signal(signal_id, wrapper, this);
    }

    template<typename T>
    template<typename Function_t>
    void has_motion_enter_signal<T>::connect_signal_motion_enter(Function_t function)
    {
        _function = [f = function](T* instance, float x, float y)
        {
            return f(instance, x, y);
        };
        static_cast<SignalEmitter*>(_instance)->connect_signal(signal_id, wrapper, this);
    }

    // MOTION

    template<typename T>
    void has_motion_signal<T>::wrapper(void*, float x, float y, has_motion_signal<T>* instance)
    {
        if (instance->_blocked)
            instance->_function(instance->_instance, x, y);
    }

    template<typename T>
    template<typename Function_t, typename Data_t>
    void has_motion_signal<T>::connect_signal_motion(Function_t function, Data_t data)
    {
        _function = [f = function, d = data](T* instance, float x, float y)
        {
            return f(instance, x, y);
        };
        static_cast<SignalEmitter*>(_instance)->connect_signal(signal_id, wrapper, this);
    }

    template<typename T>
    template<typename Function_t>
    void has_motion_signal<T>::connect_signal_motion(Function_t function)
    {
        _function = [f = function](T* instance, float x, float y)
        {
            return f(instance, x, y);
        };
        static_cast<SignalEmitter*>(_instance)->connect_signal(signal_id, wrapper, this);
    }
}