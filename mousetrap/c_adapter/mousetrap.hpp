//
// Copyright (c) Clemens Cords (mail@clemens-cords.com), created 3/27/23
//

#pragma once

#include <cstdint>

#ifdef CLANG_JL
    #undef __cplusplus
#endif

#ifndef __cplusplus
extern "C" {
#endif

void test();

#ifndef __cplusplus
} // end of extern "C"
#endif