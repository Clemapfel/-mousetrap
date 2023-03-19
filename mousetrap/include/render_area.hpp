//
// Copyright (c) Clemens Cords (mail@clemens-cords.com), created 3/19/23
//

#pragma once

#include <include/widget.hpp>
#include <include/shape.hpp>
#include <include/render_task.hpp>

namespace mousetrap
{
    class RenderArea : public WidgetImplementation<GtkGLArea>//,
        // HAS_SIGNAL(RenderArea, realize),
        //HAS_SIGNAL(RenderArea, render),
        //HAS_SIGNAL(RenderArea, resize)
    {
        public:
            RenderArea();

            void add_render_task(Shape*, Shader* = nullptr, GLTransform* = nullptr, BlendMode = BlendMode::NORMAL);
            void add_render_task(RenderTask);
            void clear_render_tasks();

            void queue_render();
            void make_current();

        private:
            static void on_resize(RenderArea* area, gint width, gint height, void*);
            static gboolean on_render(RenderArea*, GdkGLContext*, void*);
            std::vector<RenderTask> _render_tasks;
    };
}