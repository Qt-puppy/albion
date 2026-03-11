cmake_minimum_required(VERSION 3.10)
project(Albion CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(/opt/homebrew/include)
include_directories(/opt/homebrew/include/SDL2)
link_directories(/opt/homebrew/lib)

# 1. Tell CMake where to find ImGui headers
include_directories(${CMAKE_SOURCE_DIR}/vendor/imgui)
include_directories(${CMAKE_SOURCE_DIR}/vendor/imgui/backends)

# 2. Add ImGui source files to your compile list
set(IMGUI_SOURCES
    vendor/imgui/imgui.cpp
    vendor/imgui/imgui_draw.cpp
    vendor/imgui/imgui_tables.cpp
    vendor/imgui/imgui_widgets.cpp
    vendor/imgui/backends/imgui_impl_sdl2.cpp
    vendor/imgui/backends/imgui_impl_sdlrenderer2.cpp
)

# 3. Compile Albion + ImGui together
add_executable(Albion src/main.cpp ${IMGUI_SOURCES})

target_link_libraries(Albion PRIVATE SDL2 SDL2_image)