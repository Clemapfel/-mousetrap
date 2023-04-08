//
// Copyright (c) Clemens Cords (mail@clemens-cords.com), created 4/9/23
//

#pragma once

#include <include/gtk_common.hpp>

namespace mousetrap
{
    /// @brief glib managed memory
    class Object
    {
        public:
            /// @brief increase the reference count of the object by 1
            /// @see https://docs.gtk.org/gobject/method.Object.ref.html
            void ref() const;

            /// @brief decrease the reference count of the object by 1
            /// @see https://docs.gtk.org/gobject/method.Object.unref.html
            void unref() const;

        protected:
            /// @brief expose the internal gobject, this may no necessarily be the same as SignalEmitter::operator GObject*()
            /// @return pointer to GObject
            virtual GObject* get_internal() const = 0;
    };
}
