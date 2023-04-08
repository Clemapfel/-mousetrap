//
// Created by clem on 4/9/23.
//

#include <include/object.hpp>
#include <include/log.hpp>
namespace mousetrap
{
    void Object::ref() const
    {
        auto* self = get_internal();
        if (g_object_is_floating(self))
        {
            log::warning("In Object::ref: Attempting to ref a floating reference. Use of floating references is discouraged");
            g_object_ref_sink(self);
        }
        else
        {
            g_object_ref(self);
        }
    }

    void Object::unref() const
    {
        auto* self = get_internal();
        if (G_IS_OBJECT(self))
            g_object_unref(get_internal());
        else
            log::warning("In Object::unref: Object is already deallocated or Object::get_internal failed to return a GObject*");
    }
}