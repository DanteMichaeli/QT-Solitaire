# Solitaire

## Introduction

This project is a simple C++ Solitaire game. The project is built using **CMake** and can be compiled and run on multiple platforms (Linux, macOS, Windows) using either **GCC** or **Clang**.

## Prerequisites

**Note:** These commands use the syntax of bash / zsh shells. For others, consult alternative sources.

Before building and running the project, make sure the following are installed on your system:

- A C++ compiler (e.g., **GCC**, **Clang**).
- **CMake** (version 3.10 or higher).

You can verify that these are installed with the following commands:

```bash
gcc --version    # For GCC
clang --version  # For Clang
cmake --version  # To check CMake version
```

## Instructions

### 1. **Configure the Project**

Before building the project, you need to configure it. This generates the necessary build files (such as `Makefile`) based on the `CMakeLists.txt` file.

Run the following command:

```bash
cmake -S . -B build
```

### 2. **Build the Project**

After configuring the project, you can now compile the source files into an executable.

Run the following command:

```bash
cmake --build build
```

This compiles the project and generates the executable in the `build` directory.

### 3. **Run the Executable**

Once the build is complete, you can run the program. The compiled executable will be located in the `build` directory.

Run the following command to execute it:

```bash
./build/solitaire
```

- Make sure your compiler supports **C++17**.

## For Developers

Follow the same instructions as above for setup. When making changes, rerun the last two **build** and **run** commands to see your changes in output. Some IDE extensions may automatically do some of this for you, so you might find easier ways of doing this.

When making larger changes, e.g. importing libraries, switching compilers, et.c., it can be a good idea to clean the project before rebuilding. Run:

```bash
cmake --build build --target clean
```

After this, build and run normally.
