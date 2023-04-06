//
// Created by clem on 4/6/23.
//

#include <julia.h>
#include <mousetrap.hpp>
#include <jlcxx/jlcxx.hpp>

namespace mousetrap
{
    template<typename... Ts>
    void log_critical(Ts... ts)
    {
        std::stringstream str;
        auto append = [](std::stringstream& str, auto x){
            str << x;
        };
        (append(str, ts), ...);
        log::critical(str.str(), MOUSETRAP_DOMAIN);
    }

    template<typename T>
    inline std::string jl_to_string(T x)
    {
        static auto* string = jl_get_function(jl_base_module, "string");
        auto* out = jl_call1(string, static_cast<jl_value_t*>(x));
        return out == nullptr ? "" : jl_string_ptr(out);
    }
}
