using Clang.Generators

cd(@__DIR__)

include_dir = normpath("../mousetrap/c_adapter")
options = load_options(joinpath(@__DIR__, "generator.toml"))

args = get_default_args()

for str in [
    "-I/home/clem/Workspace/rat_game/mousetrap/include",
    "-DCLANG_JL" # set to ignore parts of code with #ifnef CLANG_JL
]
    push!(args, str)
end

headers = [joinpath(include_dir, header) for header in readdir(include_dir) if endswith(header, ".hpp") | endswith(header, ".h")]
context = create_context(headers, args, options)
build!(context)
