//
// Copyright (c) Clemens Cords (mail@clemens-cords.com), created 4/3/23
//

#pragma once

#include <gtk/gtk.h>
#include <iostream>
#include <sstream>

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

    /// @brief generate a new gobject wrapper
    /// @param T CamelCase identifier of the value to be wrapped, this has to be an already valid class name
    /// @param t snake_case identifier of the value to be wrapped
    /// @param T_CAPS CAPS_CASE identifier of the vaue to be wrapped
    /// @example
    /// \code
    ///     #define G_TYPE_TEST_WRAPPER (test_wrapper_get_type())
    ///     G_NEW_WRAPPER(Test, test, TEST)
    /// \endcode
    #define G_NEW_WRAPPER(T, t, T_CAPS) \
        G_DECLARE_FINAL_TYPE(T##Wrapper, t##_wrapper, G, T_CAPS##_WRAPPER, GObject) \
                                        \
        \
        struct _##T##Wrapper \
        { \
            GObject parent_instance; \
            T* data; \
        }; \
        \
        struct _##T##Wrapper##Class \
        { \
            GObjectClass parent_class; \
        }; \
        \
        G_DEFINE_TYPE (T##Wrapper, t##_wrapper, G_TYPE_OBJECT) \
        \
        static void t##_wrapper_finalize (GObject *object) \
        { \
            auto* self = G_##T_CAPS##_WRAPPER(object);                              \
                                        \
            if (not log::get_surpress_debug(MOUSETRAP_DOMAIN))                      \
            {\
                std::stringstream str; str << "In " << #t << "_wrapper_finalize: Deallocating object of type " << #T << " at " << self->data; \
                log::debug(str.str(), MOUSETRAP_DOMAIN);                            \
            }\
            delete self->data;          \
            G_OBJECT_CLASS(t##_wrapper_parent_class)->finalize(object); \
        } \
        \
        static void t##_wrapper_init(T##Wrapper * item) \
        { \
            item->data = nullptr; \
        } \
        \
        static void t##_wrapper_class_init(T##Wrapper##Class* c) \
        { \
            GObjectClass *gobject_class = G_OBJECT_CLASS (c); \
             gobject_class->finalize = t##_wrapper_finalize; \
        } \
        \
        static T##Wrapper* t##_wrapper_new(T* in) \
        { \
            auto* item = (T##Wrapper*) g_object_new(G_TYPE_##T_CAPS##_WRAPPER, nullptr); \
            t##_wrapper_init(item); \
            item->data = in; \
            return item; \
        }                               \
                                        \
        static GObject* wrap(T* in)         \
        {       \
            return G_OBJECT(t##_wrapper_new(in));                                \
        }                                \

    namespace internal
    {
        /// @brief function that releases attached gobject ref when objects reference count reaches 0
        /// @param attachment attached object
        /// @param object parent object
        /// @param gboolean true if triggered while parent has exactly 1 reference left
        static void toggle_notify(gpointer attachment, GObject* object, gboolean last_ref)
        {
            if (last_ref and G_IS_OBJECT(attachment))
            {
                g_object_unref(attachment);
                g_object_remove_toggle_ref(object, toggle_notify, attachment);
            }
        }
    }

    /// @brief attach a gobject wrapper to an arbitrary GObject parent. If parents reference count reaches 0, the attachment is freed.
    /// @param parent parent object, if this object goes out of scope, attachment is deleted too
    /// @param attachment object to be deleted when parent goes out of scope
    static void attach_ref_to_object(GObject* parent, GObject* attachment)
    {
        g_object_add_toggle_ref(parent, internal::toggle_notify, attachment);
    }

    /// @brief detach a gobject wrapper from an arbitrary GObject parent
    /// @param parent parent object, if this object goes out of scope, attachment is deleted too
    /// @param attachment object to be deleted when parent goes out of scope
    static void detach_ref_from_object(GObject* parent, GObject* attachment)
    {
        g_object_remove_toggle_ref(parent, internal::toggle_notify, attachment);
    }
}