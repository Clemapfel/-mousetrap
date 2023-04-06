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

    template<typename... Args_t>
    jl_value_t* jl_safe_call(jl_function_t* function, Args_t... in)
    {
        static auto* jl_safe_call = jl_eval_string(R"(
            function safe_call(f::Function, args...)

                res::Any = undef

                backtrace::String = ""
                exception_occurred::Bool = false
                exception::Union{Exception, UndefInitializer} = undef

                try
                    res = f(args...)
                catch e
                    exception = e
                    backtrace = sprint(Base.showerror, exception, catch_backtrace())
                    exception_occurred = true
                end

                return (res, exception_occurred, exception, backtrace)
            end
        )");

        static auto* jl_throw = jl_get_function(jl_base_module, "throw");

        static std::array<jl_value_t*, sizeof...(Args_t) + 1> args;
        static auto set = [&](size_t i, jl_value_t* x) {args[i] = x;};

        args[0] = (jl_value_t*) function;

        size_t i = 1;
        (set(i++, (jl_value_t*) in), ...);

        auto* tuple_res = jl_call(jl_safe_call, args.data(), args.size());

        if (jl_unbox_bool(jl_get_nth_field(tuple_res, 1)))
        {
            log::critical(jl_to_string(jl_get_nth_field(tuple_res, 3)), MOUSETRAP_DOMAIN);
        }

        auto* res = jl_get_nth_field(tuple_res, 0);
        return res;
    }
}
