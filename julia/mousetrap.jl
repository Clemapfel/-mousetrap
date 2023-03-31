module mousetrap
  using CxxWrap
  @wrapmodule("./libmousetrap_julia_binding.so")

  function __init__()
    @initcxx
  end
end

println(mousetrap.hello_world());