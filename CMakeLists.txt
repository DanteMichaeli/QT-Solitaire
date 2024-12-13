cmake_minimum_required(VERSION 3.10)
project(solitaire VERSION 1.0 LANGUAGES CXX)

# Set C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable Qt's auto-moc, auto-rcc, and auto-uic (for Qt's meta-object compiler, resources, and UI compiler)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

# Find the required Qt components (Qt 6 in your case)
find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)

# Include directories
include_directories(${Qt6Widgets_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/src)  # Add this line to include the src directory
# Add the miniaudio directory to the include path
include_directories(${CMAKE_SOURCE_DIR}/libs/miniaudio)

# Automatically find all .cpp / .hpp / .c / .h files in the src/ directory and subdirectories
file(GLOB_RECURSE SOURCES_C_CPP "src/*.c" "src/*.cpp")
file(GLOB_RECURSE HEADERS "src/*.h" "src/*.hpp")


# Combine all sources
set(SOURCES ${SOURCES_C_CPP} ${HEADERS})

# Add the resources.qrc file to the project
qt6_add_resources(RESOURCES src/assets/resources.qrc)

# Add the executable for the main application
add_executable(solitaire ${SOURCES} ${RESOURCES})

# Link the Qt libraries automatically
target_link_libraries(solitaire Qt6::Core dl Qt6::Gui Qt6::Widgets)

# Enable verbose output for CMake
set(CMAKE_VERBOSE_MAKEFILE ON)

# Export compile commands for use with tools like clang-tidy
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
#
## Add Catch2 as a subdirectory (Catch2 is now located under the libs/ folder as a git submodule)
# Add Catch2 as a subdirectory
add_subdirectory(libs/Catch2)

set(TEST_SOURCES
    ${CMAKE_SOURCE_DIR}/tests/test_card.cpp
    ${CMAKE_SOURCE_DIR}/tests/test_deck.cpp
    ${CMAKE_SOURCE_DIR}/tests/test_dummy.cpp
    ${CMAKE_SOURCE_DIR}/tests/test_klondikePile.cpp
    ${CMAKE_SOURCE_DIR}/tests/test_targetPile.cpp
    ${CMAKE_SOURCE_DIR}/tests/test_wastePile.cpp
    ${CMAKE_SOURCE_DIR}/tests/test_game.cpp
)

# Add test sources
list(APPEND TEST_SOURCES 
    ${CMAKE_SOURCE_DIR}/src/card.cpp
    ${CMAKE_SOURCE_DIR}/src/pile.cpp
    ${CMAKE_SOURCE_DIR}/src/klondikePile.cpp
    ${CMAKE_SOURCE_DIR}/src/wastePile.cpp
    ${CMAKE_SOURCE_DIR}/src/targetPile.cpp
    ${CMAKE_SOURCE_DIR}/src/deck.cpp
    ${CMAKE_SOURCE_DIR}/src/game.cpp
    ${CMAKE_SOURCE_DIR}/src/gui/gameSoundManager.cpp
    ${CMAKE_SOURCE_DIR}/src/stats.cpp

)

# Create the test executable
add_executable(solitaire_tests ${TEST_SOURCES})

# Link necessary libraries
target_link_libraries(solitaire_tests PRIVATE 
    Qt6::Core 
    Qt6::Gui 
    Qt6::Widgets
    Catch2::Catch2WithMain
    ${RESOURCES}
)

# Include directories
target_include_directories(solitaire_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/libs/Catch2/src
    ${CMAKE_SOURCE_DIR}/src  # Include src directory for headers
)

# Enable AUTOMOC for Qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
