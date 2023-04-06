module mousetrap
    using CxxWrap

    @wrapmodule("./libmousetrap_julia_binding.so")

    function __init__()
    @initcxx
    end
end

println(mousetrap.Widget <: mousetrap.SignalEmitter)

exit(0)
for n in names(mousetrap; all = true)
    println(n, " ", typeof(getproperty(mousetrap, n)))
end