# Solitaire

## Introduction

This project is a simple C++ Solitaire game. The project is built using **CMake** and can be compiled and run on multiple platforms (Linux, macOS, Windows) using either **GCC** or **Clang**.

## Prerequisites

Ensure you have the following installed on your system:

1. **C++ Compiler**

   - Install a compiler that supports C++17 or later (e.g., GCC, Clang).

2. **Qt Framework (6.x.x)**

   - Download and install Qt 6.x.x from the [official Qt website](https://www.qt.io/download).
   - Necessary for building (Application can be also run using Docker)

3. **CMake**

   - Install CMake (version 3.10 or later).

4. **Git**

   - Install Git to clone the repository.

## Cloning the Repository

1. Open a terminal or command prompt.
2. Clone the repository:
   ```bash
   git clone git@version.aalto.fi:runner1/qt-soliatire.git
   ```
3. Navigate to the project directory:
   ```bash
   cd qt-soliaire
   ```
   (SIC)
4. Update all the git submodules.
   ```bash
   git submodule update --init --recursive
   ```

---

## Building the Project using CMake

1. Create a configured build directory:
   ```bash
    cmake -DCMAKE_PREFIX_PATH="<path-to-Qt/6.x.x/gcc_64>" -S . -B build
   ```
   Replace the `<path-to-Qt/6.x.x/gcc_64>` with the path to your Qt6 installation.
2. Navigate to the build directory
   ```bash
   cd build
   ```
3. Configure CMAKE:
   ```bash
   cmake ..
   ```
4. Build the program
   ```bash
   make
   ```
5. Navigate back to project folder
   ```bash
   cd ..
   ```
6. Run the application:
   ```bash
   ./build/solitaire
   ```

---

## Running the application with Docker

The program can also be run within a docker container removing the necessity of Qt6 installation.

### Purpose

Using Docker to run the Solitaire program ensures a consistent environment with all necessary dependencies, regardless of the host operating system.

### Steps to Run

- Example with linux machines

1. Install Docker:

   ```bash
   sudo apt install -y docker.io
   ```

2. Allow local display access

   ```bash
   xhost +local:docker
   ```

3. Run the Docker container with display access:
   ```bash
   sudo docker run -e DISPLAY=$DISPLAY -v /tmp/.X11-unix:/tmp/.X11-unix -e LANG=C.UTF-8 dantemichaelii/qtsolitaire-linux
   ```

### What It Does

- The Docker container includes the Qt framework and all dependencies required to run the Solitaire application.
- Running the container launches the Solitaire game and connects to the host's display server for GUI rendering.
