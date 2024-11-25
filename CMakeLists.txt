cmake_minimum_required(VERSION 3.10)
project(solitaire VERSION 1.0 LANGUAGES CXX)

# Set C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable Qt's auto-moc, auto-rcc, and auto-uic (for Qt's meta-object compiler, resources, and UI compiler)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find the required Qt components (Qt 6 in your case)
find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)

# Include directories
include_directories(${Qt6Widgets_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/src)  # Add this line to include the src directory

# Automatically find all .cpp / .hpp / .c / .h files in the src/ directory and subdirectories
file(GLOB_RECURSE SOURCES_C_CPP "src/*.c" "src/*.cpp")
file(GLOB_RECURSE HEADERS "src/*.h" "src/*.hpp")

# Combine all sources
set(SOURCES ${SOURCES_C_CPP} ${HEADERS})

# Add the resources.qrc file to the project
qt6_add_resources(RESOURCES src/assets/resources.qrc)

# Add the executable
add_executable(solitaire ${SOURCES} ${RESOURCES})

# Link the Qt libraries automatically
target_link_libraries(solitaire Qt6::Core Qt6::Gui Qt6::Widgets)

# Enable verbose output for CMake
set(CMAKE_VERBOSE_MAKEFILE ON)

# Export compile commands for use with tools like clang-tidy
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)