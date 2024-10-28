cmake_minimum_required(VERSION 3.10)
project(solitaire VERSION 1.0 LANGUAGES CXX)

# Set C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable Qt's auto-moc, auto-rcc, and auto-uic (for Qt's meta-object compiler, resources, and UI compiler)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# In case of older CMake versions
if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

# Find the required Qt components (Qt 6 in your case)
find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)

# Automatically find all .cpp files in the src/ directory and subdirectories
file(GLOB_RECURSE SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp")

# Add the executable
add_executable(solitaire ${SOURCES})

# Link the Qt libraries automatically
target_link_libraries(solitaire Qt6::Core Qt6::Gui Qt6::Widgets)

# Enable verbose output for CMake
set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)