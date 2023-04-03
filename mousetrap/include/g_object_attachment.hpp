//
// Copyright (c) Clemens Cords (mail@clemens-cords.com), created 4/3/23
//

#pragma once

#include <gtk/gtk.h>
#include <iostream>

namespace mousetrap::detail
{
    struct Test
    {
        size_t _i = -1;

        Test(size_t i)
            : _i(i)
        {}

        ~Test()
        {
            std::cout << "destroyed " << _i << std::endl;
        }
    };

    #define G_TYPE_WRAPPER (wrapper_get_type())

    G_DECLARE_FINAL_TYPE(Wrapper, wrapper, G, WRAPPER, GObject)

    struct _Wrapper
    {
        GObject parent_instance;
        Test* data;
    };

    struct _WrapperClass
    {
        GObjectClass parent_class;
    };

    G_DEFINE_TYPE (Wrapper, wrapper, G_TYPE_OBJECT)

    static void wrapper_finalize (GObject *object)
    {
        auto* self = G_WRAPPER(object);
        delete self->data;
        G_OBJECT_CLASS(wrapper_parent_class)->finalize(object);
    }

    static void wrapper_init(Wrapper* item)
    {
        item->data = nullptr;
    }

    static void wrapper_class_init(WrapperClass* c)
    {
        GObjectClass *gobject_class = G_OBJECT_CLASS (c);
        gobject_class->finalize = wrapper_finalize;
    }

    static Wrapper* wrapper_new(Test* in)
    {
        auto* item = (Wrapper*) g_object_new(G_TYPE_WRAPPER, nullptr);
        wrapper_init(item);
        item->data = in;
        return item;
    }
}