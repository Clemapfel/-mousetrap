//
// Copyright 2022 Clemens Cords
// Created on 7/16/22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <vector>
#include <string>
#include <algorithm>
#include <mutex>

#include <include/gl_common.hpp>
#include <include/shader.hpp>
#include <include/color.hpp>
#include <include/gl_transform.hpp>
#include <include/texture.hpp>
#include <include/geometry.hpp>

namespace mousetrap
{
    /// @brief opengl shape primitive, variable number of vertices
    class Shape
    {
        public:
            /// @brief ctor, allocates vertex buffer gpu-side
            Shape();

            /// @brief dtor, deallocated vertex buffer gpu-side
            ~Shape();

            /// @brief copy ctor, allocates new vertex array with identical vertex data
            /// @param other
            Shape(const Shape&);

            /// @brief copy assignment, allocates new vertex array with identical vertex data
            /// @param other
            /// @returns reference to self after assignment
            Shape& operator=(const Shape&);

            /// @brief move ctor
            /// @param other
            Shape(Shape&&) noexcept;

            /// @brief move assignment
            /// @param other
            /// @returns reference to self after assignment
            Shape& operator=(Shape&&) noexcept;

            /// @brief construct as 2d point that point is always rendered as exactly 1 fragment
            /// @param position position normalized 2d space
            void as_point(Vector2f);

            /// @brief construct as set of points, each point is always rendered as exactly 1 fragment
            /// @param points vector of points
            void as_points(const std::vector<Vector2f>&);

            /// @brief construct as filled triangle
            /// @param a point in normalized 2d space
            /// @param b point in normalized 2d space
            /// @param c point in normalized 2d space
            void as_triangle(Vector2f a, Vector2f b, Vector2f c);

            /// @brief construct as filled rectangle from top left and size
            /// @param top_left point in normalized 2d space
            /// @param size length in normalized 2d space
            void as_rectangle(Vector2f top_left, Vector2f size);

            /// @brief construct as filled rectangle from 4 points
            /// @param a point in normalized 2d space
            /// @param b point in normalized 2d space
            /// @param c point in normalized 2d space
            /// @param d point in normalized 2d space
            void as_rectangle(Vector2f, Vector2f, Vector2f, Vector2f);

            /// @brief construct as filled circle
            /// @param center point in normalized 2d space
            /// @param radius distance in normalized 2d space
            /// @param n_outer_vertices number of equally spaced vertices on the perimeter of the circle
            void as_circle(Vector2f center, float radius, size_t n_outer_vertices);

            /// @brief construct as ellipse
            /// @param center point in normalized 2d space
            /// @param x_radius radius along the x-axis, normalized distance in 2d space
            /// @param y_radius radius along the y-axis, normalized distance in 2d space
            /// @param n_outer_vertices number of equally spaced vertices on the perimeter of the circle
            void as_ellipse(Vector2f center, float x_radius, float y_radius, size_t n_outer_vertices);

            /// @brief construct as line, has a width of 1 fragment exactly
            /// @param a point in normalized 2d space
            /// @param b point in normalized 2d space
            void as_line(Vector2f a, Vector2f b);

            /// @brief construct as set of lines, each has a width of exactly 1 fragment
            /// @param points vector of pairs of 2 points, both in normalized 2d space
            void as_lines(const std::vector<std::pair<Vector2f, Vector2f>>&);

            /// @brief construct as set of connected lines
            /// @param points {a1, a2, ..., an} will result in line segments {a1, a2}, {a2, a3}, ..., {an-1, an}
            void as_line_strip(const std::vector<Vector2f>&);

            /// @brief construct as convex polygon
            /// @param points points in normalized 2d space, minimum bounding polygon is calculated on these, so some of the vertices ay be discarded
            void as_polygon(const std::vector<Vector2f>& positions);

            /// @brief construct as rectanglular frame of given thickness
            /// @param top_left top left anchor of the outer perimeter of the frame, in normalized 2d space
            /// @param outer_size width and height of the oute perimeter of the frame, in normalized 2d space
            /// @param x_width horizontal width of the frames inner bounds, in normalized 2d space
            /// @param y_width vertical height of the frames inner bounds, in normalized 2d space
            void as_rectangle_frame(Vector2f top_left, Vector2f outer_size, float x_width, float y_width);

            /// @brief construct as circular ring, a "2d donut"
            /// @param center center in 2d space
            /// @param outer_radius radius from the center to the outer perimeter of the ring
            /// @param thickness width of the rings inner bounds along all axis
            /// @param n_outer_vertices number of vertices on the outer perimeter of the ring
            void as_circular_ring(Vector2f center, float outer_radius, float thickness, size_t n_outer_vertices);

            /// @brief construct as elliptic ring, a "2d elliptic donut"
            /// @param center center in 2d space
            /// @param x_radius horizontal radius of the outer perimeter of the ellipse
            /// @param y_radius vertical radius of the outer perimeter of the ellipse
            /// @param x_thickness width of the inner bound of the ellipse along the x-axis
            /// @param y_thickness height of the inner bounds of the ellipse along the y-axis
            /// @param n_outer_vertices number of vertices on the outer perimeter of the ellipse
            void as_elliptic_ring(Vector2f center, float x_radius, float y_radius, float x_thickness, float y_thickness, size_t n_outer_vertices);

            /// @brief construct as a closed loop linesegment
            /// @param points {a1, a2, ..., an} will result in line segments {a1, a2}, {a2, a3}, ..., {an-1, an}, {an, a1}
            void as_wireframe(const std::vector<Vector2f>&);

            /// @brief construct a wireframe from a shapes outer vertices. Useful for generating frames or outlines
            /// @param shape another shape, will generate minimum bounding polygon and construct a wireframe from that polygon
            void as_wireframe(const Shape&);

            /// @brief render the shape to the currently bound framebuffer
            /// @param shader shader program to use
            /// @param transform transform to hand to the vertex shader
            void render(const Shader& shader, GLTransform transform) const;

            /// @brief get color of n-th vertex
            /// @returns color as RGBA
            RGBA get_vertex_color(size_t) const;
            void set_vertex_color(size_t, RGBA);

            void set_vertex_texture_coordinate(size_t, Vector2f);
            Vector2f get_vertex_texture_coordinate(size_t) const;

            void set_vertex_position(size_t, Vector3f);
            Vector3f get_vertex_position(size_t) const;

            size_t get_n_vertices() const;

            void set_color(RGBA);

            void set_visible(bool);
            bool get_visible() const;

            Rectangle get_bounding_box() const;
            Vector2f get_size() const;

            void set_centroid(Vector2f);
            Vector2f get_centroid() const;

            void set_top_left(Vector2f);
            Vector2f get_top_left() const;

            void rotate(Angle);

            void set_texture(const TextureObject*);
            const TextureObject* get_texture() const;

        private:
            struct Vertex
            {
                Vertex(float x, float y, RGBA rgba)
                : position(x, y, 0), color(rgba), texture_coordinates(0, 0)
                {}

                Vector3f position;
                RGBA color;
                Vector2f texture_coordinates;
            };

            RGBA _color = RGBA(1, 1, 1, 1);
            bool _visible = true;

            std::vector<Vertex> _vertices;
            std::vector<int> _indices;
            GLenum _render_type = GL_TRIANGLE_STRIP;

            void update_position() const;
            void update_color() const;
            void update_texture_coordinate() const;
            void initialize();

            std::vector<Vector2f> sort_by_angle(const std::vector<Vector2f>&);

            struct VertexInfo
            {
                float _position[3];
                float _color[4];
                float _texture_coordinates[2];
            };

            void update_data(
                bool update_position = true,
                bool update_color = true,
                bool update_tex_coords = true
            ) const;

            mutable std::vector<VertexInfo> _vertex_data;

            GLNativeHandle _vertex_array_id = 0,
            _vertex_buffer_id = 0;

            const TextureObject* _texture = nullptr;
    };
}

