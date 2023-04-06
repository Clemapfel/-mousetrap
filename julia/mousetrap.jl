module mousetrap
    using CxxWrap

    struct Vector2{T}
        x::T
        y::T
    end
    const Vector2f = Vector2{Float32}
    const Vector2i = Vector2{Int64}
    const Vector2ui = Vector2{UInt64}

    struct Vector3{T}
        x::T
        y::T
        z::T
    end
    const Vector3f = Vector3{Float32}
    const Vector3i = Vector3{Int64}
    const Vector3ui = Vector3{UInt64}

    struct Vector4{T}
        x::T
        y::T
        z::T
        w::T
    end
    const Vector4f = Vector4{Float32}
    const Vector4i = Vector4{Int64}
    const Vector4ui = Vector4{UInt64}

    @wrapmodule("./libmousetrap_julia_binding.so")

    function __init__()
    @initcxx
    end
end

app = mousetrap.Application("test.app")
window = Ref{mousetrap.Window}()
mousetrap.connect_signal_activate(app, function(app::mousetrap.Application)
end)

println(mousetrap.run(app))

exit(0)

for n in names(mousetrap; all = true)
    #printstyled(n; bold=true); printstyled("\t", typeof(getproperty(mousetrap, n)), "\n")
end

