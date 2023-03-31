//
// Created by clem on 3/31/23.
//

#include "julia_binding.hpp"

std::string hello_world()
{
    return "hello world";
}

JLCXX_MODULE define_julia_module(jlcxx::Module& module)
{
    module.method("hello_world", &hello_world);
}
