#ifndef GAME_H
#define GAME_H

#include "Grid.h"
#include <list>
#include <utility>

template<typename T>
class Game {
private:
    Grid<T> grid;
    std::list<std::pair<int, int>> snake; // Coordinates of the snake
    std::pair<int, int> direction; // Current direction of movement
    std::pair<int, int> foodPosition;
    bool isGameOver;

    T snakeSymbol;
    T appleSymbol;
    T emptySymbol;

public:
    Game(int width, int height, T snakeSymbol, T appleSymbol, T emptySymbol);

    void initializeGame(); // Initialize grid, snake, and food
    void placeFood(); // Randomly place food on the grid
    void moveSnake(); // Move the snake in the current direction
    bool checkCollision(const std::pair<int, int>& newHead) const; // Check for collisions
    void update(); // Update the game state
    bool getGameOver() const; // Check if the game is over

    void setDirection(int dx, int dy); // Update snake's direction
    void displayGrid() const; // Display grid (console-based for now)
};

#endif // GAME_H
