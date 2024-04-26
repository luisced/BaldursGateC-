cmake_minimum_required(VERSION 3.29)
project(RPG)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/helper)
include_directories(${CMAKE_SOURCE_DIR}/classes)

# Find ncurses
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

# Include nlohmann_json via FetchContent
include(FetchContent)
FetchContent_Declare(
  json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.10.5  # specify the version you need
)
FetchContent_MakeAvailable(json)

# Define your executable and specify all source files
add_executable(project
  main.cpp
  helper/error.cpp
  helper/screen.cpp
  classes/dungeon.cpp
  classes/json_conversion.cpp
  classes/game.cpp
  classes/player.cpp
  classes/character.cpp
  classes/location.cpp
  classes/enemy.cpp
  helper/functions.cpp
  classes/item.cpp
)

# Link libraries
target_link_libraries(project PRIVATE ${CURSES_LIBRARIES} nlohmann_json)
