cmake_minimum_required(VERSION 3.12)
project(rat_game VERSION 0.0.1 LANGUAGES CXX C)

## DEPS

find_library(OpenGL REQUIRED NAMES GL)
find_library(GLEW REQUIRED NAMES GLEW)
find_package(SFML COMPONENTS audio REQUIRED)

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

        mousetrap/include/music.hpp
        mousetrap/src/music.cpp

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
        mousetrap/src/action.cpp
        mousetrap/src/action.inl

        mousetrap/include/frame_clock.hpp
        mousetrap/src/frame_clock.cpp

        mousetrap/include/event_controller.hpp
        mousetrap/src/event_controller.cpp

        mousetrap/include/key_event_controller.hpp
        mousetrap/src/key_event_controller.cpp

        mousetrap/include/motion_event_controller.hpp
        mousetrap/src/motion_event_controller.cpp

        mousetrap/include/aspect_frame.hpp
        mousetrap/src/aspect_frame.cpp

        mousetrap/include/box.hpp
        mousetrap/src/box.cpp

        mousetrap/include/button.hpp
        mousetrap/src/button.cpp

        mousetrap/include/check_button.hpp
        mousetrap/src/check_button.cpp

        mousetrap/include/toggle_button.hpp
        mousetrap/src/toggle_button.cpp

        mousetrap/include/entry.hpp
        mousetrap/src/entry.cpp

        mousetrap/include/expander.hpp
        mousetrap/src/expander.cpp

        mousetrap/include/revealer.hpp
        mousetrap/src/revealer.cpp

        mousetrap/include/frame.hpp
        mousetrap/src/frame.cpp

        mousetrap/include/image_display.hpp
        mousetrap/src/image_display.cpp

        mousetrap/include/key_file.hpp
        mousetrap/src/key_file.cpp

        mousetrap/include/label.hpp
        mousetrap/src/label.cpp

        mousetrap/include/justify_mode.hpp

        mousetrap/include/text_view.hpp
        mousetrap/src/text_view.cpp

        mousetrap/include/list_view.hpp
        mousetrap/src/list_view.cpp

        mousetrap/include/selection_model.hpp
        mousetrap/src/selection_model.cpp

        mousetrap/include/render_area.hpp

        mousetrap/include/click_event_controller.hpp
        mousetrap/src/click_event_controller.cpp

        mousetrap/include/scroll_event_controller.hpp
        mousetrap/src/scroll_event_controller.cpp

        mousetrap/include/focus_event_controller.hpp
        mousetrap/src/focus_event_controller.cpp

        mousetrap/include/drag_event_controller.hpp
        mousetrap/src/drag_event_controller.cpp

        mousetrap/include/pinch_zoom_event_controller.hpp
        mousetrap/src/pinch_zoom_event_controller.cpp

        mousetrap/include/rotate_event_controller.hpp
        mousetrap/src/rotate_event_controller.cpp

        mousetrap/include/adjustment.hpp
        mousetrap/src/adjustment.cpp

        mousetrap/include/scrollbar.hpp
        mousetrap/src/scrollbar.cpp

        mousetrap/include/viewport.hpp
        mousetrap/src/viewport.cpp

        mousetrap/include/scrolled_window.hpp
        mousetrap/src/scrolled_window.cpp

        mousetrap/include/grid_view.hpp
        mousetrap/src/grid_view.cpp

        mousetrap/include/menu_model.hpp
        mousetrap/src/menu_model.cpp

        mousetrap/include/popover_menu_button.hpp
        mousetrap/src/popover_menu_button.cpp

        mousetrap/include/popover.hpp
        mousetrap/src/popover.cpp

        mousetrap/include/relative_position.hpp

        mousetrap/src/render_area.cpp

        mousetrap/include/center_box.hpp
        mousetrap/src/center_box.cpp

        mousetrap/include/clipboard.hpp
        mousetrap/src/clipboard.cpp
        mousetrap/src/clipboard.inl

        mousetrap/include/stack.hpp
        mousetrap/src/stack.cpp

        mousetrap/include/transition_type.hpp

        mousetrap/include/notebook.hpp
        mousetrap/src/notebook.cpp

        mousetrap/include/column_view.hpp
        mousetrap/src/column_view.cpp

        mousetrap/include/spin_button.hpp
        mousetrap/src/spin_button.cpp

        mousetrap/include/drop_down.hpp
        mousetrap/src/drop_down.cpp
        mousetrap/src/drop_down.inl

        mousetrap/include/fixed.hpp
        mousetrap/src/fixed.cpp

        mousetrap/include/level_bar.hpp
        mousetrap/src/level_bar.cpp

        mousetrap/include/spinner.hpp
        mousetrap/src/spinner.cpp

        mousetrap/include/progress_bar.hpp
        mousetrap/src/progress_bar.cpp

        mousetrap/include/paned.hpp
        mousetrap/src/paned.cpp

        mousetrap/include/scale.hpp
        mousetrap/src/scale.inl
        mousetrap/src/scale.cpp

        mousetrap/include/overlay.hpp
        mousetrap/src/overlay.cpp

        mousetrap/include/separator.hpp
        mousetrap/src/separator.cpp

        mousetrap/include/switch.hpp
        mousetrap/src/switch.cpp

        mousetrap/include/file_system.hpp
        mousetrap/src/file_system.cpp

        mousetrap/include/file_descriptor.hpp
        mousetrap/src/file_descriptor.cpp

        mousetrap/include/file_monitor.hpp
        mousetrap/src/file_monitor.inl
        mousetrap/src/file_monitor.cpp

        mousetrap/include/orientable.hpp

        mousetrap/include/grid.hpp
        mousetrap/src/grid.cpp

        mousetrap/include/selectable.hpp
        mousetrap/src/spin_button.inl

        mousetrap/include/music.hpp
        mousetrap/src/music.cpp

        mousetrap/include/icon.hpp
        mousetrap/src/icon.cpp

        mousetrap/include/log.hpp
        mousetrap/src/log.cpp

        mousetrap/include/file_chooser.hpp
        mousetrap/src/file_chooser.cpp
        mousetrap/src/file_chooser.cpp)

target_include_directories(mousetrap PUBLIC
    "${CMAKE_SOURCE_DIR}/mousetrap"
    "${GTK_INCLUDE_DIRS}"
)

target_link_libraries(mousetrap PUBLIC
    ${OpenGL}
    ${GLEW}
    ${GTK_LIBRARIES}
    ${sfml-audio}
)

target_compile_features(mousetrap PUBLIC cxx_std_20)
set_target_properties(mousetrap PROPERTIES
    LINKER_LANGUAGE CXX
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}
)

## TESTS

option(MOUSETRAP_BUILD_TESTING "Build all tests" ON)
if(MOUSETRAP_BUILD_TESTING)
    set(TEST_PREFIX "test_")
    function(declare_test name)
        add_executable(
            "${TEST_PREFIX}${name}"
            "mousetrap/test/${name}.cpp"
            "mousetrap/test/test.hpp"
        )
        target_link_libraries("${TEST_PREFIX}${name}" PRIVATE mousetrap)
        add_test(NAME "${TEST_PREFIX}${name}" COMMAND "${TEST_PREFIX}${name}")
    endfunction()

    declare_test(application)
    declare_test(window)
    declare_test(action)
endif()

## JULIA BINDING

find_package(JlCxx REQUIRED)
get_target_property(JlCxx_location JlCxx::cxxwrap_julia LOCATION)
get_filename_component(JlCxx_location ${JlCxx_location} DIRECTORY)

add_library(mousetrap_julia_binding SHARED
    julia/julia_binding.cpp
)

target_link_libraries(mousetrap_julia_binding PUBLIC
    ${mousetrap}
    JlCxx::cxxwrap_julia
)

target_include_directories(mousetrap_julia_binding PUBLIC
    ${CMAKE_SOURCE_DIR}/mousetrap
    "${GTK_INCLUDE_DIRS}"
)

set_target_properties(mousetrap_julia_binding PROPERTIES
    LINKER_LANGUAGE CXX
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/julia
)

## RAT_GAME

find_library(lua REQUIRED NAMES lua)
set(SOL_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/sol2/include")

add_executable(debug main.cpp)
target_link_libraries(debug PRIVATE
    mousetrap
    ${lua}
)

target_include_directories(debug PRIVATE
    "${CMAKE_SOURCE_DIR}"
    "${SOL_INCLUDE_DIR}"
)
