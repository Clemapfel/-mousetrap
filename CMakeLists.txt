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

set(MOUSETRAP_HEADER_FILES
    mousetrap/include/shape.hpp
    mousetrap/include/gl_common.hpp
    mousetrap/include/vector.hpp
    mousetrap/include/shader.hpp
    mousetrap/include/color.hpp
    mousetrap/include/gl_transform.hpp
    mousetrap/include/angle.hpp
    mousetrap/include/texture.hpp
    mousetrap/include/image.hpp
    mousetrap/include/geometry.hpp
    mousetrap/include/render_texture.hpp
    mousetrap/include/render_task.hpp
    mousetrap/include/blend_mode.hpp
    mousetrap/include/texture_object.hpp
    mousetrap/include/scale_mode.hpp
    mousetrap/include/wrap_mode.hpp
    mousetrap/include/msaa_render_texture.hpp
    mousetrap/include/alignment.hpp
    mousetrap/include/orientation.hpp
    mousetrap/include/sound_buffer.hpp
    mousetrap/include/sound.hpp
    mousetrap/include/time.hpp
    mousetrap/include/music.hpp
    mousetrap/include/signal_emitter.hpp
    mousetrap/src/signal_emitter.inl
    mousetrap/include/signal_component.hpp
    mousetrap/include/application.hpp
    mousetrap/include/window.hpp
    mousetrap/include/widget.hpp
    mousetrap/src/widget.inl
    mousetrap/include/cursor_type.hpp
    mousetrap/include/action.hpp
    mousetrap/src/action.inl
    mousetrap/include/frame_clock.hpp
    mousetrap/include/event_controller.hpp
    mousetrap/include/key_event_controller.hpp
    mousetrap/include/motion_event_controller.hpp
    mousetrap/include/aspect_frame.hpp
    mousetrap/include/box.hpp
    mousetrap/include/button.hpp
    mousetrap/include/check_button.hpp
    mousetrap/include/toggle_button.hpp
    mousetrap/include/entry.hpp
    mousetrap/include/expander.hpp
    mousetrap/include/revealer.hpp
    mousetrap/include/frame.hpp
    mousetrap/include/image_display.hpp
    mousetrap/include/key_file.hpp
    mousetrap/include/label.hpp
    mousetrap/include/justify_mode.hpp
    mousetrap/include/text_view.hpp
    mousetrap/include/list_view.hpp
    mousetrap/include/selection_model.hpp
    mousetrap/include/render_area.hpp
    mousetrap/include/click_event_controller.hpp
    mousetrap/include/scroll_event_controller.hpp
    mousetrap/include/focus_event_controller.hpp
    mousetrap/include/drag_event_controller.hpp
    mousetrap/include/pinch_zoom_event_controller.hpp
    mousetrap/include/rotate_event_controller.hpp
    mousetrap/include/adjustment.hpp
    mousetrap/include/scrollbar.hpp
    mousetrap/include/viewport.hpp
    mousetrap/include/scrolled_window.hpp
    mousetrap/include/grid_view.hpp
    mousetrap/include/menu_model.hpp
    mousetrap/include/popover_menu_button.hpp
    mousetrap/include/popover.hpp
    mousetrap/include/relative_position.hpp
    mousetrap/include/center_box.hpp
    mousetrap/include/clipboard.hpp
    mousetrap/src/clipboard.inl
    mousetrap/include/stack.hpp
    mousetrap/include/transition_type.hpp
    mousetrap/include/notebook.hpp
    mousetrap/include/column_view.hpp
    mousetrap/include/spin_button.hpp
    mousetrap/include/drop_down.hpp
    mousetrap/src/drop_down.inl
    mousetrap/include/fixed.hpp
    mousetrap/include/level_bar.hpp
    mousetrap/include/spinner.hpp
    mousetrap/include/progress_bar.hpp
    mousetrap/include/paned.hpp
    mousetrap/include/scale.hpp
    mousetrap/src/scale.inl
    mousetrap/include/overlay.hpp
    mousetrap/include/separator.hpp
    mousetrap/include/switch.hpp
    mousetrap/include/file_system.hpp
    mousetrap/include/file_descriptor.hpp
    mousetrap/include/file_monitor.hpp
    mousetrap/include/orientable.hpp
    mousetrap/include/grid.hpp
    mousetrap/include/selectable.hpp
    mousetrap/src/spin_button.inl
    mousetrap/include/music.hpp
    mousetrap/include/icon.hpp
    mousetrap/include/log.hpp
    mousetrap/src/log.inl
    mousetrap/include/file_chooser.hpp
    mousetrap/include/shortcut_controller.hpp
    mousetrap/include/menubar.hpp
        mousetrap/include/long_press_event_controller.hpp mousetrap/include/pan_event_controller.hpp)

set(MOUSETRAP_SOURCE_FILES
    mousetrap/src/shape.cpp
    mousetrap/src/gl_common.cpp
    mousetrap/src/shader.cpp
    mousetrap/src/color.cpp
    mousetrap/src/gl_transform.cpp
    mousetrap/src/angle.cpp
    mousetrap/src/texture.cpp
    mousetrap/src/image.cpp
    mousetrap/src/geometry.cpp
    mousetrap/src/render_texture.cpp
    mousetrap/src/render_task.cpp
    mousetrap/src/blend_mode.cpp
    mousetrap/src/msaa_render_texture.cpp
    mousetrap/src/sound_buffer.cpp
    mousetrap/src/sound.cpp
    mousetrap/src/time.cpp
    mousetrap/src/music.cpp
    mousetrap/src/signal_emitter.cpp
    mousetrap/src/application.cpp
    mousetrap/src/window.cpp
    mousetrap/src/widget.cpp
    mousetrap/src/action.cpp
    mousetrap/src/frame_clock.cpp
    mousetrap/src/event_controller.cpp
    mousetrap/src/key_event_controller.cpp
    mousetrap/src/motion_event_controller.cpp
    mousetrap/src/aspect_frame.cpp
    mousetrap/src/box.cpp
    mousetrap/src/button.cpp
    mousetrap/src/check_button.cpp
    mousetrap/src/toggle_button.cpp
    mousetrap/src/entry.cpp
    mousetrap/src/expander.cpp
    mousetrap/src/revealer.cpp
    mousetrap/src/frame.cpp
    mousetrap/src/image_display.cpp
    mousetrap/src/key_file.cpp
    mousetrap/src/label.cpp
    mousetrap/src/text_view.cpp
    mousetrap/src/list_view.cpp
    mousetrap/src/selection_model.cpp
    mousetrap/src/click_event_controller.cpp
    mousetrap/src/scroll_event_controller.cpp
    mousetrap/src/focus_event_controller.cpp
    mousetrap/src/drag_event_controller.cpp
    mousetrap/src/pinch_zoom_event_controller.cpp
    mousetrap/src/rotate_event_controller.cpp
    mousetrap/src/adjustment.cpp
    mousetrap/src/scrollbar.cpp
    mousetrap/src/viewport.cpp
    mousetrap/src/scrolled_window.cpp
    mousetrap/src/grid_view.cpp
    mousetrap/src/menu_model.cpp
    mousetrap/src/popover_menu_button.cpp
    mousetrap/src/popover.cpp
    mousetrap/src/render_area.cpp
    mousetrap/src/center_box.cpp
    mousetrap/src/clipboard.cpp
    mousetrap/src/stack.cpp
    mousetrap/src/notebook.cpp
    mousetrap/src/column_view.cpp
    mousetrap/src/column_view.inl
    mousetrap/src/spin_button.cpp
    mousetrap/src/drop_down.cpp
    mousetrap/src/fixed.cpp
    mousetrap/src/level_bar.cpp
    mousetrap/src/spinner.cpp
    mousetrap/src/progress_bar.cpp
    mousetrap/src/paned.cpp
    mousetrap/src/scale.cpp
    mousetrap/src/overlay.cpp
    mousetrap/src/separator.cpp
    mousetrap/src/switch.cpp
    mousetrap/src/file_system.cpp
    mousetrap/src/file_descriptor.cpp
    mousetrap/src/file_monitor.cpp
    mousetrap/src/grid.cpp
    mousetrap/src/music.cpp
    mousetrap/src/icon.cpp
    mousetrap/src/log.cpp
    mousetrap/src/file_chooser.cpp
    mousetrap/src/file_chooser.cpp
    mousetrap/src/shortcut_controller.cpp
    mousetrap/src/signal_component.cpp
    mousetrap/src/menu_bar.cpp mousetrap/src/long_press_event_controller.cpp mousetrap/src/pan_event_controller.cpp)

add_library(mousetrap SHARED
   ${MOUSETRAP_HEADER_FILES}
   ${MOUSETRAP_SOURCE_FILES}
)

target_include_directories(mousetrap PUBLIC
    "${CMAKE_SOURCE_DIR}/mousetrap"
    ${GTK_INCLUDE_DIRS}
)

target_link_libraries(mousetrap PUBLIC
    ${OpenGL}
    ${GLEW}
    ${GTK_LIBRARIES}
    sfml-audio
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
    declare_test(action)

    add_executable(docs_screenshots mousetrap/docs/widgets/widgets.cpp)
    target_link_libraries(docs_screenshots PRIVATE mousetrap)
endif()

## JULIA BINDING

find_package(JlCxx REQUIRED)
get_target_property(JlCxx_location JlCxx::cxxwrap_julia LOCATION)
get_filename_component(JlCxx_location ${JlCxx_location} DIRECTORY)

add_library(mousetrap_julia_binding SHARED
    julia/julia_binding.cpp
    julia/julia_interface.cpp
    ${MOUSETRAP_SOURCE_FILES}
)

target_link_libraries(mousetrap_julia_binding PUBLIC
    ${mousetrap}
    JlCxx::cxxwrap_julia
    ${OpenGL}
    ${GLEW}
    ${GTK_LIBRARIES}
    ${sfml-audio}
)

target_include_directories(mousetrap_julia_binding PUBLIC
    ${CMAKE_SOURCE_DIR}/mousetrap
    "${GTK_INCLUDE_DIRS}"
)

set_target_properties(mousetrap_julia_binding PROPERTIES
    LINKER_LANGUAGE CXX
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/julia
    POSITION_INDEPENDENT_CODE 1
)

add_compile_definitions(mousetrap_julia_binding MOUSETRAP_JULIA_BINDING)

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
