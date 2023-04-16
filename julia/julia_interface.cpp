//
// Copyright (c) Clemens Cords (mail@clemens-cords.com), created 4/16/23
//

#pragma once

#include <julia.h>
#include <string>

jl_value_t* jl_get_property(jl_value_t* value, const std::string& name)
{
    static auto* get_property = jl_get_function(jl_base_module, "getproperty");
    static auto* has_property = jl_get_function(jl_base_module, "hasproperty");

    if (not jl_unbox_bool(jl_call2(has_property, value, (jl_value_t*) jl_symbol(name.c_str()))))
        return jl_nothing;

    return jl_call2(get_property, value, (jl_value_t*) jl_symbol(name.c_str()));
}

bool jl_assert_type(jl_value_t* value, const std::string& type)
{
    auto* type_v = jl_get_global(jl_main_module, jl_symbol(type.c_str()));
    return jl_isa(value, type_v);
}

jl_value_t* jl_box_string(const std::string& in)
{
    static auto* string = jl_get_global(jl_base_module, jl_symbol("string"));
    return jl_call1(string, (jl_value_t*) jl_symbol(in.c_str()));
}

void jl_throw_exception(const std::string& message)
{
    static auto* exception = jl_get_global(jl_base_module, jl_symbol("ErrorException"));
    jl_throw( jl_call1(exception, jl_box_string(message)));
}

template<typename... T>
jl_value_t* jl_calln(jl_function_t* function, T... args)
{
    std::array<jl_value_t*, sizeof...(T)> wrapped = {args...};
    return jl_call(function, wrapped.data(), wrapped.size());
}