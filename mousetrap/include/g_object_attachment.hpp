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

    // WRAPPER IMPLEMENTATION

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

    /// @brief function that releases attached gobject ref when objects reference count reaches 0
    /// @param data attached object
    /// @param object parent object
    /// @param gboolean true if triggered while parent has exactly 1 reference left
    static void toggle_notify(gpointer data, GObject* object, gboolean last_ref)
    {
        if (last_ref)
        {
            g_object_unref(data);
            g_object_remove_toggle_ref(object, toggle_notify, data);
        }
    }

    /// @brief attach a gobject wrapper to an arbitrary GObject parent. If parents reference count reaches 0, the attachment is freed.
    /// @param parent parent object, if this object goes out of scope, attachment is deleted too
    /// @param attachment if this object goes out of scope, parent is unaffected
    static void attach_ref_to_object(GObject* parent, GObject* attachment)
    {
        g_object_add_toggle_ref(parent, toggle_notify, attachment);
    }
}