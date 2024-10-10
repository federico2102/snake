#include "Game.h"
#include "Renderer.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

bool isFirstRender = true;  // Track if it's the first render

// Constructor
template<typename T>
Game<T>::Game(int width, int height, T snakeSymbol, T appleSymbol, T emptySymbol)
    : grid(width, height, emptySymbol), previousGrid(width, height, emptySymbol),
        direction({0, 1}), isGameOver(false),
      snakeSymbol(snakeSymbol), appleSymbol(appleSymbol), emptySymbol(emptySymbol) {
    initializeGame();
}

// Initialize game elements
template<typename T>
void Game<T>::initializeGame() {
    // Initialize the snake in the center
    snake = {{grid.getWidth() / 2, grid.getHeight() / 2}};
    grid.setCell(snake.front().first, snake.front().second, snakeSymbol);
    previousGrid = grid; // Initialize previousGrid to match current grid

    // Place the first piece of food
    placeFood();
}

// Place food randomly on the grid
template<typename T>
void Game<T>::placeFood() {
    srand(time(nullptr));
    int x, y;
    do {
        x = rand() % grid.getWidth();
        y = rand() % grid.getHeight();
    } while (grid.getCell(x, y) == snakeSymbol); // Ensure it's not placed on the snake

    foodPosition = {x, y};
    grid.setCell(x, y, appleSymbol); // Use the appleSymbol for food
}

// Move the snake in the current direction
template<typename T>
void Game<T>::moveSnake() {
    auto newHead = std::make_pair(snake.front().first + direction.first,
                                  snake.front().second + direction.second);

    // Wrap around the grid (circular)
    newHead.first = (newHead.first + grid.getWidth()) % grid.getWidth();
    newHead.second = (newHead.second + grid.getHeight()) % grid.getHeight();

    if (checkCollision(newHead)) {
        isGameOver = true;
        return;
    }

    // Move the snake: Add new head
    snake.push_front(newHead);
    grid.setCell(newHead.first, newHead.second, snakeSymbol); // Set the new head as snake

    // Check if food is eaten
    if (newHead == foodPosition) {
        placeFood(); // Place new food
    } else {
        // Remove the tail
        auto tail = snake.back();
        snake.pop_back();
        grid.setCell(tail.first, tail.second, emptySymbol); // Clear tail (set as empty)
    }
}

// Check for collisions with itself
template<typename T>
bool Game<T>::checkCollision(const std::pair<int, int>& newHead) const {
    return grid.getCell(newHead.first, newHead.second) == snakeSymbol; // Check if it hits the snake
}

// Update game state (move snake, check food, check collision)
template<typename T>
void Game<T>::update() {
    if (!isGameOver) {
        previousGrid = grid; // Store the previous state of the grid before updating
        moveSnake();
    }
}

// Set the direction of the snake
template<typename T>
void Game<T>::setDirection(int dx, int dy) {
    direction = {dx, dy};
}

// Display the grid
template<typename T>
void Game<T>::displayGrid() {
    Renderer::printGrid(grid, previousGrid, isFirstRender);  // The renderer will display the grid with the chosen symbols
    isFirstRender = false;
    previousGrid = grid;
}

// Get the game-over state
template<typename T>
bool Game<T>::getGameOver() const {
    Renderer::moveCursorBelowGrid(grid.getHeight());  // Move cursor two lines below the grid
    return isGameOver;
}

// Explicit template instantiation for char (if needed)
template class Game<char>;
