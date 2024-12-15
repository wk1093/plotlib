cmake_minimum_required(VERSION 3.29)

project(plot)

set(CMAKE_CXX_STANDARD 17)

add_executable(plot src/main.cpp)

include(FetchContent)

# A plotting library with a simple API similar to Python's matplotlib
# uses raylib as a backend, with a custom shader optimized to plot and graph data
FetchContent_Declare(
  raylib
  GIT_REPOSITORY https://github.com/raysan5/raylib.git
  GIT_TAG master
  GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(raylib)

target_link_libraries(plot raylib)