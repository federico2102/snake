#ifndef GAME_H
#define GAME_H

#include "Grid.h"
#include <list>
#include <utility>
#include <vector>  // For multiple apple positions

// Max number of keys we track
#define KEY_COUNT 256
class CheatHandler;
class InputHandler;

class Game {
private:
    Grid<char> grid;
    Grid<char> previousGrid;

    std::list<std::pair<int, int>> snake1; // Coordinates of snake 1
    std::pair<int, int> direction1; // Current direction of snake 1

    std::list<std::pair<int, int>> snake2; // Coordinates of snake 2
    std::pair<int, int> direction2; // Current direction of snake 2

    std::vector<std::pair<int, int>> foodPositions; // Multiple apple positions
    bool isGameOver;
    bool isMultiplayer;

    int applesEaten; // Track the total number of apples eaten
    int snakeSpeed; // Snake speed in milliseconds
    int playerWhoLost; // 1 for Player 1, 2 for Player 2

    char snakeSymbol1;
    char snakeSymbol2;  // Symbol for second snake
    char appleSymbol;
    char emptySymbol;

    int maxApplesOnMap; // Max number of apples that can be on the map
    bool paradiseModeActive; // Flag for paradise mode
    bool keys[KEY_COUNT]{}; // Boolean array to track keypress states

    friend class CheatHandler;  // Allow CheatHandler access to private members
    friend class InputHandler;

public:
    Game(int width, int height, char snakeSymbol1, char snakeSymbol2, char appleSymbol, char emptySymbol, bool isMultiplayer);

    void initializeGame(); // Initialize grid, snakes, and food
    void generateFoodPositions(int numApples); // Generate multiple apples on the grid
    void moveSnake(std::list<std::pair<int, int>>& snake, const std::pair<int, int>& direction, char snakeSymbol, int playerNumber); // Move the snakes
    static bool checkCollision(const std::pair<int, int>& newHead, const std::list<std::pair<int, int>>& snake); // Check for collisions
    void update(); // Update the game state
    bool getGameOver() const; // Check if the game is over and which player lost
    void start();

    void setDirection1(int dx, int dy); // Update direction for snake 1
    void setDirection2(int dx, int dy); // Update direction for snake 2
    void displayGrid(bool &isFirstRender); // Display grid (console-based for now)
};

#endif // GAME_H
