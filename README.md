# Parallel Mandelbrot Explorer

This project is an interactive Mandelbrot set viewer, implemented in C++ using SDL2 for rendering, OpenMP for parallel computation, and CMake for build configuration. The viewer supports zooming, panning, and changing color palettes for exploring the Mandelbrot set in real-time.

## Features

- **Interactive Viewer**: Navigate and zoom in/out of the Mandelbrot set using intuitive keyboard and mouse controls.
- **Parallel Rendering**: Accelerated computations using OpenMP to leverage multi-core CPUs.
- **Color Palettes**: Multiple color schemes, including classic, fire, electric, ocean, and grayscale.
- **Customizable Parameters**: Modify rendering parameters like maximum iterations and zoom levels.

## Prerequisites

- **C++ Compiler**: C++17 or later (e.g., GCC, Clang, or MSVC).
- **Libraries**:
  - [SDL2](https://www.libsdl.org/) (Simple DirectMedia Layer)
  - OpenMP (for parallelism)
- **Build Tool**: CMake

## Build Instructions

1. **Clone the Repository**:
   bash
   git clone <repository_url>
   cd <repository_folder>
2. *** Build with CMake:***
 mkdir build
cd build
cmake ..
make
3. ***Run the Application:***
 ./ParallelMandelbrotExplorer
Controls
Navigation:

Arrow keys: Move up, down, left, or right.

Zoom:

+ or scroll up: Zoom in.

- or scroll down: Zoom out.

Color Palette:

Press C: Cycle through available color palettes.

Reset:

Press R: Reset the view to the default position and zoom.

Help:

Press H: Display control instructions in the console.

Quit:

Press ESC or Q: Exit the application.

Mandelbrot Set Parameters
MAX_ITERATIONS: Controls the number of iterations for convergence testing (default: 1000).

Zoom: Determines the magnification level.

OffsetX/OffsetY: Adjusts the center of the view.

Customization
Modify the source file (parallel Mandelbrot.cpp) to adjust default parameters or add new features. Key parameters include:

SCREEN_WIDTH and SCREEN_HEIGHT: Window dimensions.

MAX_ITERATIONS: Number of iterations to calculate Mandelbrot divergence.

Example Screenshots
Include screenshots or gifs showcasing various zoom levels and color palettes.

Acknowledgments
SDL2 for rendering.

OpenMP for parallel processing.
