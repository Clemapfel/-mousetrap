//
// Created by clem on 3/31/23.
//

#include <mousetrap.hpp>
#include <jlcxx/jlcxx.hpp>

using namespace mousetrap;


JLCXX_MODULE define_types_module(jlcxx::Module& types)
{
    using namespace jlcxx;

    types.add_type<Parametric<TypeVar<1>, TypeVar<2>>>("TemplateType")
    .apply<TemplateType<int, int>, TemplateType<int, int>>([](auto wrapped)
    {
     typedef typename decltype(wrapped)::type WrappedT;
     wrapped.method("get_first", &WrappedT::get_first);
     wrapped.method("get_second", &WrappedT::get_second);
    });
}

JLCXX_MODULE mousetrap_define(jlcxx::Module& module)
{
    module.add_type<Application>("Application")
        .constructor<const ApplicationID&>()
        .method("run", &Application::run)
    ;
}
