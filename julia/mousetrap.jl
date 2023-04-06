module mousetrap
    using CxxWrap

    @wrapmodule("./libmousetrap_julia_binding.so")

    function __init__()
    @initcxx
    end
end

for n in names(mousetrap; all = true)
    #printstyled(n; bold=true); printstyled("\t", typeof(getproperty(mousetrap, n)), "\n")
end

exit(0)
