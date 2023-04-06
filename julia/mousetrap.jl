module mousetrap
    using CxxWrap

    @wrapmodule("./libmousetrap_julia_binding.so")

    function __init__()
    @initcxx
    end

    function test(x::Bool) ::Bool
        println("test")
        return x
    end

    action = Action("test")
    println(get_state(action))
    set_stateful_function(action, test, true)
    activate(action)
    println(get_state(action))
end


exit(0)

for n in names(mousetrap; all = true)
    #printstyled(n; bold=true); printstyled("\t", typeof(getproperty(mousetrap, n)), "\n")
end

