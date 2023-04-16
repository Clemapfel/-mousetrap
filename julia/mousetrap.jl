module mousetrap

    """
    export all enum instances into current module
    """
    macro export_enum(enum)
        quote
            export $enum
            for i in instances($enum)
                export i
            end
        end
    end

    module detail
        using CxxWrap
        function __init__()
            @initcxx
        end
        @wrapmodule("./libmousetrap_julia_binding.so")
    end

    """
    """
    struct RGBA
        r::Float32
        g::Float32
        b::Float32
        a::Float32
    end
    export RGBA

    """
    """
    struct HSVA
        h::Float32
        s::Float32
        v::Float32
        a::Float32
    end
    export HSVA

    """
    """
    function rgba_to_hsva(rgba::RGBA) ::HSVA

        h = Ref{Float32}(0)
        s = Ref{Float32}(0)
        v = Ref{Float32}(0)
        a = Ref{Float32}(0)

        detail.rgba_to_hsva(rgba.r, rgba.g, rgba.b, rgba.a, h, s, v, a)
        return HSVA(h[], s[], v[], a[])
    end
    export rgba_to_hsva

    """
    """
    function hsva_to_rgba(hsva::HSVA) ::RGBA

        r = Ref{Float32}(0)
        g = Ref{Float32}(0)
        b = Ref{Float32}(0)
        a = Ref{Float32}(0)

        detail.hsva_to_rgba(hsva.h, hsva.s, hsva.v, hsva.a, r, g, b, a)
        return RGBA(r[], g[], b[], a[])
    end
    export hsva_to_rgba

    """
    """
    invert(rgba::RGBA) ::RGBA = RGBA(1 - rgba.r, 1 - rgba.g, 1 - rgba.b, rgba.a)
    invert(hsva::HSVA) ::HSVA = HSVA(1 - hsva.h, hsva.s, 1 - hsva.v, hsva.a)
    export invert

    """
    """
    const Image = detail.Image
    export Image

    """
    """
    create(image::Image, x::Unsigned, y::Unsigned) = detail.create(image, convert(UInt64, x), convert(UInt64, y))
    export create

    """
    """
    const create_from_file = detail.create_from_file
    export create_from_file

    """
    """
    const save_to_file = detail.save_to_file
    export save_to_file

    """
    """
    const get_size = detail.get_size
    export get_size

    """
    """
    const InterpolationType = detail.InterpolationType
    @export_enum InterpolationType

    """
    """
    as_scaled(image::Image, new_width::Unsigned, new_height::Unsigned) = detail.as_scaled(image, UInt64(new_with), UInt64(new_height))
    export as_scaled

    """
    """
    as_scaled(image::Image, offset_x::Integer, offset_y::Integer, new_width::Unsigned, new_height::Unsigned) = detail.as_cropped(image, Int32(offset_x), Int32(offset_y), UInt64(new_with), UInt64(new_height))
    export as_scaled
end

using .mousetrap
using Test

for n in names(mousetrap) println(n) end