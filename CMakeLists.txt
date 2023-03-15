cmake_minimum_required(VERSION 3.12)
project(rat_game VERSION 0.0.1 LANGUAGES CXX)

## DEPS

find_library(OpenGL REQUIRED NAMES GL)
find_library(GLEW REQUIRED NAMES GLEW)
find_package(SFML COMPONENTS system window graphics network audio REQUIRED)
include(CheckIncludeFileCXX)

find_package(PkgConfig)
pkg_check_modules(GTK gtk4)

if (NOT GTK_FOUND)
    message(FATAL_ERROR "Missing Dependency: unable to locate gtk4")
endif()

CHECK_INCLUDE_FILE_CXX(glm/glm.hpp GLM_FOUND)
if(NOT GLM_FOUND)
    message(WARNING "Missing Dependency: glm headers were not detected")
endif()

## CONFIGURE

set(RESOURCE_PATH "${CMAKE_SOURCE_DIR}/resources/")
configure_file(
        "${CMAKE_SOURCE_DIR}/mousetrap/include/resource_path.hpp.in"
        "${CMAKE_SOURCE_DIR}/mousetrap/include/resource_path.hpp"
        IMMEDIATE @ONLY
)

## LIBRARY

add_library(mousetrap STATIC
    mousetrap/include/shape.hpp
    mousetrap/src/shape.cpp

    mousetrap/include/gl_common.hpp
    mousetrap/src/gl_common.cpp

    mousetrap/include/vector.hpp

    mousetrap/include/shader.hpp
    mousetrap/src/shader.cpp

        mousetrap/include/color.hpp
        mousetrap/src/color.cpp

    mousetrap/include/gl_transform.hpp
    mousetrap/src/gl_transform.cpp

    mousetrap/include/angle.hpp
    mousetrap/src/angle.cpp

    mousetrap/include/texture.hpp
    mousetrap/src/texture.cpp

    mousetrap/include/image.hpp
    mousetrap/src/image.cpp

    mousetrap/include/geometry.hpp
    mousetrap/src/geometry.cpp

    mousetrap/include/render_texture.hpp
    mousetrap/src/render_texture.cpp

    mousetrap/include/render_task.hpp
    mousetrap/src/render_task.cpp

    mousetrap/include/blend_mode.hpp
    mousetrap/src/blend_mode.cpp

    mousetrap/include/texture_object.hpp
    mousetrap/include/resource_path.hpp.in

    mousetrap/include/scale_mode.hpp
    mousetrap/include/wrap_mode.hpp
        mousetrap/include/msaa_render_texture.hpp mousetrap/src/msaa_render_texture.cpp mousetrap/include/signals.hpp mousetrap/include/alignable.hpp mousetrap/src/alignable.cpp mousetrap/include/resizable.hpp mousetrap/src/resizable.cpp mousetrap/include/movable.hpp mousetrap/include/widget.hpp mousetrap/src/widget.cpp mousetrap/include/rectangle_shape.hpp mousetrap/src/rectangle_shape.cpp mousetrap/include/box.hpp mousetrap/include/orientation.hpp mousetrap/src/box.cpp mousetrap/include/overlay.hpp mousetrap/src/overlay.cpp)

target_include_directories(mousetrap PUBLIC
    "${CMAKE_SOURCE_DIR}/mousetrap"
    "${GTK_INCLUDE_DIRS}"
)

target_link_libraries(mousetrap PUBLIC
    ${OpenGL}
    ${GLEW}
    ${GTK_LIBRARIES}
    sfml-window
)

target_compile_features(mousetrap PUBLIC cxx_std_20)
set_target_properties(mousetrap PROPERTIES
    LINKER_LANGUAGE CXX
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}
)

## EXECUTABLE

add_executable(debug main.cpp)
target_link_libraries(debug PUBLIC mousetrap)
target_include_directories(debug PUBLIC "${CMAKE_SOURCE_DIR}")