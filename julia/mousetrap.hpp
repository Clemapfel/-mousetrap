//
// Copyright (c) Clemens Cords (mail@clemens-cords.com), created 3/27/23
//

#pragma once

#ifndef CLANG_JL
extern "C" {
#endif

void inside_extern_c();
void outside_extern_c();

#ifndef CLANG_JL
}
#endif
