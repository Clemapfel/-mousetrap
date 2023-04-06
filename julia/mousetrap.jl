module mousetrap
    using CxxWrap

    @wrapmodule("./libmousetrap_julia_binding.so")

    function __init__()
    @initcxx
    end
end

for n in names(mousetrap; all = true)
    println(n, " ", typeof(getproperty(mousetrap, n)))
end