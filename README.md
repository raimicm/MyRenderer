# OBJ Renderer (CPU-Based)

A Wavefront (.obj) model renderer/viewer implemented without graphics libraries (OpenGL, Vulkan, etc.).

## Dependencies

- [**SDL**](https://github.com/libsdl-org/SDL): window creation
- [**glm**](https://github.com/g-truc/glm): matrix and vector operations

## Build Instructions

#### Step 0: Requirements 

Download the following if you haven't already:
- C++17 compiler
- CMake 3.16+
- Git

#### Step 1: Clone the Repository

Clone this repository with the following command:

```bash
git clone --recurse-submodules https://github.com/raimicm/MyRenderer.git
```

#### Step 2: Build the Project

Change directory into the root folder of the cloned repository and run the following commands:

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

#### Step 3: Run the Executable

In the `build` directory, locate the executable named `objRenderer`. If you cannot find the executable, it is likely in a subdirectory of `build`. Once you have located `objRenderer`, use the following command in the directory of the executable to run the program:

```bash
./objRenderer
```
