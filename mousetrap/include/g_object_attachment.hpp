//
// Copyright (c) Clemens Cords (mail@clemens-cords.com), created 4/3/23
//

#pragma once

#include <gtk/gtk.h>
#include <iostream>
#include <sstream>
#include <memory>

#include <include/log.hpp>

namespace mousetrap::detail
{

    #define G_TYPE_RIGHT(t_snake) (t_snake##_get_type())

    #define G_WRAPPER_FORWARD_DECLARE(T) \
    static GObject* wrap(T* in);

    /// @brief generate a new gobject wrapper
    /// @param T CamelCase identifier of the value to be wrapped, this has to be an already valid class name
    /// @param t snake_case identifier of the value to be wrapped
    /// @param T_CAPS CAPS_CASE identifier of the vaue to be wrapped
    /// @example
    /// \code
    ///     using Test = /* ... */
    ///     #define G_TYPE_TEST G_WRAPPER_TYPE_RIGHT(test)
    ///     G_NEW_TYPE(Test, test, TEST)
    /// \endcode
    #define G_NEW_TYPE(T, t, T_CAPS, wrapped_type) \
        G_DECLARE_FINAL_TYPE(T, t, G, T_CAPS, GObject) \
                                        \
        \
        struct _##T \
        { \
            GObject parent_instance; \
            wrapped_type data; \
        }; \
        \
        struct _##T##Class \
        { \
            GObjectClass parent_class; \
        }; \
        \
        G_DEFINE_TYPE (T, t, G_TYPE_OBJECT) \
        \
        static void t##_finalize (GObject *object) \
        {                               \
            auto* self = G_##T_CAPS(object);                              \
                                        \
            if (not log::get_surpress_debug(MOUSETRAP_DOMAIN))                      \
            {\
                std::stringstream str; str << "In " << #t << "_finalize: Deallocating object of type " << #wrapped_type << " at " << self->data; \
                log::debug(str.str(), MOUSETRAP_DOMAIN);                            \
            }\
            delete self->data;          \
            G_OBJECT_CLASS(t##_parent_class)->finalize(object); \
        } \
        \
        static void t##_init(T * item) \
        { \
            item->data = nullptr; \
        } \
        \
        static void t##_class_init(T##Class* c) \
        { \
            GObjectClass *gobject_class = G_OBJECT_CLASS (c); \
             gobject_class->finalize = t##_finalize; \
        } \
        \
        static T* t##_new(wrapped_type in) \
        { \
            auto* item = (T*) g_object_new(G_TYPE_##T_CAPS, nullptr); \
            t##_init(item); \
            item->data = in;                       \
            if (not log::get_surpress_debug(MOUSETRAP_DOMAIN))                      \
            {\
                std::stringstream str; str << "In " << #t << "_new: Taking ownership over object of type " << #wrapped_type <<  " at " << item->data; \
                log::debug(str.str(), MOUSETRAP_DOMAIN);                            \
            }\
            return item; \
        }                               \
                                        \
        static T* wrap(wrapped_type in)         \
        {       \
            return t##_new(in);                                \
        }                                \

    namespace internal
    {
        /// @brief function that releases attached gobject ref when objects reference count reaches 0
        /// @param attachment attached object
        /// @param object parent object
        /// @param gboolean true if triggered while parent has exactly 1 reference left
        static void toggle_notify(gpointer attachment, GObject* object, gboolean last_ref)
        {
            if (last_ref)
            {
                g_object_unref(attachment);
                g_object_remove_toggle_ref(object, toggle_notify, attachment);
            }
        }
    }

    /// @brief attach a gobject wrapper to an arbitrary GObject parent. If parents reference count reaches 0, the attachment is freed.
    /// @param parent parent object, if this object goes out of scope, attachment is deleted too
    /// @param attachment object to be deleted when parent goes out of scope
    template<typename T>
    static auto attach_ref_to_object(GObject* parent, T attachment)
    {
        g_object_add_toggle_ref(parent, internal::toggle_notify, G_OBJECT(attachment));
        return attachment->data;
    }

    /// @brief detach a gobject wrapper from an arbitrary GObject parent
    /// @param parent parent object, if this object goes out of scope, attachment is deleted too
    /// @param attachment object to be deleted when parent goes out of scope
    static void detach_ref_from_object(GObject* parent, GObject* attachment)
    {
        g_object_remove_toggle_ref(parent, internal::toggle_notify, attachment);
    }
}