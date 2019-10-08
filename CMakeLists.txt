cmake_minimum_required (VERSION 3.14)

set(PROJECT_NAME "vemt")
set(EXECUTABLE_NAME "vemt-bot-cpp")

project(${PROJECT_NAME})

### Filter Settings for Visual Studio
# Build the visual studio filter same as source directories form.
function(assign_source_group)
    foreach(_source IN ITEMS ${ARGN})
        if (IS_ABSOLUTE "${_source}")
            file(RELATIVE_PATH _source_rel "${CMAKE_CURRENT_SOURCE_DIR}" "${_source}")
        else()
            set(_source_rel "${_source}")
        endif()
        get_filename_component(_source_path "${_source_rel}" PATH)
        string(REPLACE "/" "\\" _source_path_msvc "${_source_path}")
        source_group("${_source_path_msvc}" FILES "${_source}")
    endforeach()
endfunction(assign_source_group)

file(GLOB_RECURSE vemt_bot_cpp src/*.cpp)
file(GLOB_RECURSE vemt_bot_hpp src/*.hpp)


add_executable(${EXECUTABLE_NAME} ${vemt_bot_cpp} ${vemt_bot_hpp})

add_subdirectory(sleepy-discord)
add_subdirectory(lib/json11)
add_subdirectory(lib/logger)

target_link_libraries(${EXECUTABLE_NAME} json11 logger sleepy-discord)
target_include_directories(${EXECUTABLE_NAME} PUBLIC lib/json11/src lib/logger/src src)

assign_source_group(${vemt_bot_cpp})
assign_source_group(${vemt_bot_hpp})
