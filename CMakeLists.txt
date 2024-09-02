cmake_minimum_required(VERSION 3.0)
project(build)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set the CXX flags for Emscripten to support both SDL2
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_SDL=2 ")

# Add source files
file(GLOB_RECURSE SOURCES "SDL TicTacToe/*.cpp" "SDL TicTacToe/*.h")

# Preload files (no resources used in this project)
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --use-preload-plugins --preload-file \"${CMAKE_SOURCE_DIR}\\res@/res\"")

# Add include directories
include_directories("SDL2/include")

# Add executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link SDL2, SDL2_ttf, SDL2_image, SDL2_mixer and dependencies for Emscripten
target_link_libraries(${PROJECT_NAME} SDL2)
