using Test

# colors
@testset "color" begin
    rgba = RGBA(1, 0.5, 1, 0.75)
    as_hsva = rgba_to_hsva(rgba)
    as_rgba = hsva_to_rgba(as_hsva)
    @test as_rgba == rgba

    @test invert(invert(as_rgba)) == as_rgba
    @test invert(invert(as_hsva)) == as_hsva
end