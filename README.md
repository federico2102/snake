### `README.md` Template:

```md
# Snake Game

This is a console-based Snake game implemented in C++ that reuses several modules, such as `CircularMatrix`, `Grid`, and `Renderer`, originally developed for Conway's Game of Life. The game has been designed with modularity and code reuse in mind, allowing these components to be shared across different projects.

## Features

- **Customizable Symbols**: You can define symbols for the snake, apples (food), and empty grid cells when creating a new game.
- **Reusable Modules**: The game shares core components (such as the grid and matrix handling) with other projects like Conway's Game of Life.
- **Optimized Renderer**: A specialized renderer that efficiently updates only the parts of the grid that have changed, reducing the visual effect of blinking in the console.

## Table of Contents

- [Installation](#installation)
- [Usage](#usage)
- [Code Reusability](#code-reusability)
- [Renderer Implementation](#renderer-implementation)
- [License](#license)

## Installation

1. Clone this repository:
    ```bash
    git clone https://github.com/federico2102/snake.git
    cd snake
    ```

2. Make sure you have a C++ compiler that supports C++20, and that CMake is installed.

3. Build the project:
    ```bash
    mkdir build
    cd build
    cmake ..
    make
    ```

4. Run the game:
    ```bash
    ./snake
    ```

## Usage

When starting the game, you can choose whether to use default settings or customize the following parameters:
- Grid size (width and height).
- Snake, apple, and empty cell symbols.

To control the snake:
- Use the `W` key to move up.
- Use the `S` key to move down.
- Use the `A` key to move left.
- Use the `D` key to move right.

The goal of the game is to collect apples while avoiding collisions with the walls and your own tail.

## Code Reusability

This Snake game reuses several modules from the [Conway's Game of Life](https://github.com/yourusername/conways-game-of-life) project, demonstrating how modular code can be applied to different projects. The following components are shared across both games:

- **`CircularMatrix`**: A template-based data structure that provides a circular (toroidal) matrix, enabling the grid to "wrap around" at the edges.
- **`Grid`**: A generic grid system that tracks the state of each cell. In the Snake game, the grid tracks the snake's position, the apples, and empty spaces.
- **`Renderer`**: A custom rendering system that prints the grid to the console and efficiently updates only the parts of the grid that have changed.

## Renderer Implementation

The **Renderer** plays a key role in optimizing the visual performance of the game. Instead of redrawing the entire grid every frame, it only updates the cells that have changed (e.g., the snake's movement or when an apple is eaten). This greatly reduces the flicker effect commonly seen in console-based games.

Key features of the renderer:
- **Partial Grid Updates**: The renderer compares the current grid state with the previous grid state and only redraws cells that have changed.
- **Cursor Positioning**: It uses platform-specific system calls to control the cursor position and keep the game grid in place.
- **Cross-Platform Support**: Although currently implemented with Windows-specific API calls, the renderer has been designed to allow cross-platform compatibility with minor adjustments.

### Example of Renderer Usage:
```cpp
// Rendering the entire grid for the first time
Renderer::printGrid(grid, previousGrid, isFirstRender);

// On subsequent updates, only update changed cells
Renderer::printGrid(grid, previousGrid, isFirstRender);
```

By reusing the same `Renderer` in multiple projects (such as Snake and Conway's Game of Life), we maintain a consistent approach to rendering and make the system more maintainable.
