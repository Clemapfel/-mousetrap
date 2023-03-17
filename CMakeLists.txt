cmake_minimum_required(VERSION 3.12)
project(rat_game VERSION 0.0.1 LANGUAGES CXX)

## DEPS

find_library(OpenGL REQUIRED NAMES GL)
find_library(GLEW REQUIRED NAMES GLEW)
find_package(SFML COMPONENTS audio REQUIRED)
find_library(soundio REQUIRED NAMES soundio)

find_package(PkgConfig)
pkg_check_modules(GTK gtk4)

if (NOT GTK_FOUND)
    message(FATAL_ERROR "Missing Dependency: unable to locate gtk4")
endif()

include(CheckIncludeFileCXX)
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

    mousetrap/include/msaa_render_texture.hpp
    mousetrap/src/msaa_render_texture.cpp

    mousetrap/include/alignment.hpp
    mousetrap/include/orientation.hpp

    mousetrap/include/sound_buffer.hpp
    mousetrap/src/sound_buffer.cpp

    mousetrap/include/sound.hpp
    mousetrap/src/sound.cpp

    mousetrap/include/time.hpp
    mousetrap/src/time.cpp

    mousetrap/include/sound_stream.hpp
    mousetrap/src/sound_stream.cpp

    mousetrap/include/signal_emitter.hpp
    mousetrap/src/signal_emitter.inl
    mousetrap/src/signal_emitter.cpp

    mousetrap/include/signal_component.hpp

    mousetrap/include/application.hpp
    mousetrap/src/application.cpp

    mousetrap/include/window.hpp
    mousetrap/src/window.cpp

    mousetrap/include/widget.hpp
    mousetrap/src/widget.inl
    mousetrap/src/widget.cpp

    mousetrap/include/cursor_type.hpp

    mousetrap/include/action.hpp

    mousetrap/include/frame_clock.hpp mousetrap/src/frame_clock.cpp mousetrap/src/action.cpp)

target_include_directories(mousetrap PUBLIC
    "${CMAKE_SOURCE_DIR}/mousetrap"
    "${GTK_INCLUDE_DIRS}"
)

target_link_libraries(mousetrap PUBLIC
    ${OpenGL}
    ${GLEW}
    ${GTK_LIBRARIES}
    sfml-audio
    soundio
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