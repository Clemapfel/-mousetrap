//
// Copyright 2022 Clemens Cords
// Created on 8/6/22 by clem (mail@clemens-cords.com)
//

#pragma once

#include <string>
#include <include/gl_common.hpp>
#include <include/image.hpp>
#include <include/texture_object.hpp>
#include <include/wrap_mode.hpp>
#include <include/scale_mode.hpp>

namespace mousetrap
{
    class Texture : public TextureObject
    {
        public:
            Texture();
            Texture(GLNativeHandle);
            virtual ~Texture();

            Texture(const Texture&) = delete;
            Texture& operator=(const Texture&) = delete;
            Texture(Texture&&) noexcept;
            Texture& operator=(Texture&&) noexcept;

            [[nodiscard]] Image download() const;

            void bind(size_t texture_unit) const;
            void bind() const override;
            void unbind() const override;

            void create(size_t width, size_t height);
            void create_from_file(const std::string& path);
            void create_from_image(const Image&);

            void set_wrap_mode(WrapMode);
            WrapMode get_wrap_mode();

            void set_scale_mode(TextureScaleMode);
            TextureScaleMode get_scale_mode();

            Vector2i get_size() const;
            GLNativeHandle get_native_handle() const;

        private:
            GLNativeHandle _native_handle = 0;
            WrapMode _wrap_mode = WrapMode::STRETCH;
            TextureScaleMode _scale_mode = TextureScaleMode::NEAREST;

            Vector2i _size;
    };
}
