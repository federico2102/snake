#include "Game.h"
#include <conio.h>
#include "Renderer.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <thread>

// Constructor
template<typename T>
Game<T>::Game(int width, int height, T snakeSymbol1, T snakeSymbol2, T appleSymbol, T emptySymbol, bool isMultiplayer)
    : grid(width, height, emptySymbol), previousGrid(width, height, emptySymbol),
      direction1({0, 1}), direction2({0, -1}), isGameOver(false),
      isMultiplayer(isMultiplayer), applesEaten(0), snakeSpeed(200), playerWhoLost(0),
      snakeSymbol1(snakeSymbol1), snakeSymbol2(snakeSymbol2), appleSymbol(appleSymbol), emptySymbol(emptySymbol), maxApplesOnMap(1), paradiseModeActive(false) {
    initializeGame();

    // Initialize key states to false
    std::fill(std::begin(keys), std::end(keys), false);
}

// Handle key presses and releases
template<typename T>
void Game<T>::handleInput() {
    if (_kbhit()) {
        int key = _getch();
        // For arrow keys, _getch() returns 0 or 224 first, then the actual key code
        if (key == 0 || key == 224) {
            key = _getch();
        }

        // Mark key as pressed
        keys[key] = true;

        // Handle movement for Player 1 (W, A, S, D)
        switch (key) {
            case 'w': case 'W': setDirection1(-1, 0); break;  // Move up
            case 's': case 'S': setDirection1(1, 0); break;   // Move down
            case 'a': case 'A': setDirection1(0, -1); break;  // Move left
            case 'd': case 'D': setDirection1(0, 1); break;   // Move right
        }

        // Handle movement for Player 2 (Arrow keys)
        if (key == 0 || key == 224) {
            key = _getch();
            switch (key) {
                case 72: setDirection2(-1, 0); break;   // Up arrow
                case 80: setDirection2(1, 0); break;    // Down arrow
                case 75: setDirection2(0, -1); break;   // Left arrow
                case 77: setDirection2(0, 1); break;    // Right arrow
            }
        }
    }
}

// Initialize game elements
template<typename T>
void Game<T>::initializeGame() {
    // Initialize snake 1 in the center
    snake1 = {{grid.getWidth() / 2, grid.getHeight() / 2}};
    grid.setCell(snake1.front().first, snake1.front().second, snakeSymbol1);

    if (isMultiplayer) {
        // Initialize snake 2 on the opposite side if multiplayer
        snake2 = {{grid.getWidth() / 2, grid.getHeight() / 2 - 2}};
        grid.setCell(snake2.front().first, snake2.front().second, snakeSymbol2);
    }

    previousGrid = grid; // Initialize previousGrid to match current grid

    generateFoodPositions(maxApplesOnMap); // Start with one apple
}

// Generate multiple food positions (apples) on the grid
template<typename T>
void Game<T>::generateFoodPositions(int numApples) {
    foodPositions.clear(); // Clear any existing food positions

    srand(static_cast<unsigned int>(time(nullptr)) + applesEaten); // Ensure better randomness
    for (int i = 0; i < numApples; ++i) {
        int x, y;
        do {
            x = rand() % grid.getWidth();
            y = rand() % grid.getHeight();
        } while (grid.getCell(x, y) == snakeSymbol1 ||
                 (isMultiplayer && grid.getCell(x, y) == snakeSymbol2) ||
                 grid.getCell(x, y) == appleSymbol); // Ensure it's not placed on a snake or existing apple

        foodPositions.push_back({x, y});
        grid.setCell(x, y, appleSymbol); // Set the apple on the grid
    }
}

// Grow the snake by a certain number of segments
template<typename T>
void Game<T>::growSnake(std::list<std::pair<int, int>>& snake, int growth) {
    auto tail = snake.back(); // Capture the tail before growing
    for (int i = 0; i < growth; ++i) {
        snake.push_back(tail); // Add new segments at the tail position
    }
}

// Activate paradise mode (lots of apples everywhere, limited to 25% of the grid)
template<typename T>
void Game<T>::activateParadiseMode() {
    paradiseModeActive = true;

    // Limit apples to 25% of the grid size
    int maxApples = (grid.getWidth() * grid.getHeight()) / 8;
    generateFoodPositions(maxApples);
}

// Move the snake in the current direction
template<typename T>
void Game<T>::moveSnake(std::list<std::pair<int, int>>& snake, const std::pair<int, int>& direction, T snakeSymbol, int playerNumber) {
    auto newHead = std::make_pair(snake.front().first + direction.first,
                                  snake.front().second + direction.second);

    // Wrap around the grid (circular)
    newHead.first = (newHead.first + grid.getWidth()) % grid.getWidth();
    newHead.second = (newHead.second + grid.getHeight()) % grid.getHeight();

    // Check collision with itself
    if (checkCollision(newHead, snake)) {
        isGameOver = true;
        playerWhoLost = playerNumber; // Mark the player who lost
        return;
    }

    // Move the snake: Add new head
    snake.push_front(newHead);
    grid.setCell(newHead.first, newHead.second, snakeSymbol); // Set the new head as snake

    // Check if food is eaten
    auto it = std::ranges::find(foodPositions, newHead);
    if (it != foodPositions.end()) {
        applesEaten++;
        foodPositions.erase(it); // Remove the eaten apple

        // Check if all apples are eaten, generate new apples if needed
        if (foodPositions.empty()) {
            if (!paradiseModeActive) {
                // Increase the number of apples based on apples eaten
                if (applesEaten >= 20) {
                    maxApplesOnMap = 4; // Max 4 apples
                } else if (applesEaten >= 15) {
                    maxApplesOnMap = 3;
                } else if (applesEaten >= 10) {
                    maxApplesOnMap = 2;
                }

                // Randomize how many apples to place (between 1 and maxApplesOnMap)
                int numApples = (rand() % maxApplesOnMap) + 1;
                generateFoodPositions(numApples);
            }
        }

        // Increase speed every 5 apples eaten
        if (applesEaten % 5 == 0 && snakeSpeed > 50) { // Ensure speed doesn't become too fast
            snakeSpeed -= 20;  // Reduce the delay, increasing speed
        }
    } else {
        // Remove the tail
        auto tail = snake.back();
        snake.pop_back();
        grid.setCell(tail.first, tail.second, emptySymbol); // Clear tail (set as empty)
    }
}

template<typename T>
bool Game<T>::checkCollision(const std::pair<int, int>& newHead, const std::list<std::pair<int, int>>& snake) {
    for (const auto& segment : snake) {
        if (newHead == segment) {
            return true; // Collision detected
        }
    }
    return false;
}

// Check for cheat combinations
template<typename T>
void Game<T>::checkForCheats() {
    // Cheat for player 1: 'A' and 'D' pressed at the same time
    if (keys['a'] && keys['d']) {
        growSnake(snake1, 3);
        keys['a'] = false;
        keys['d'] = false;
    }

    // Cheat for player 2: Left and Right arrows pressed at the same time
    if (keys[75] && keys[77]) {  // 75 is Left arrow, 77 is Right arrow
        growSnake(snake2, 3);
        keys[75] = false;
        keys[77] = false;
    }

    // Paradise mode: '1', '2', and '3' pressed at the same time
    if (keys['1'] && keys['2'] && keys['3']) {
        activateParadiseMode();
        keys['1'] = false;
        keys['2'] = false;
        keys['3'] = false;
    }
}


// Update game state (move snakes)
template<typename T>
void Game<T>::update() {
    if (!isGameOver) {
        previousGrid = grid; // Store the previous state of the grid before updating

        // Move snake 1
        moveSnake(snake1, direction1, snakeSymbol1, 1); // Player 1 is snake 1

        // Move snake 2 if multiplayer and game is not over
        if (isMultiplayer) {
            moveSnake(snake2, direction2, snakeSymbol2, 2); // Player 2 is snake 2
        }

        // Check for cheat codes
        checkForCheats();
    }
}

// Set the direction of snake 1
template<typename T>
void Game<T>::setDirection1(int dx, int dy) {
    // Prevent the snake from reversing into itself
    if (dx != -direction1.first || dy != -direction1.second) {
        direction1 = {dx, dy};
    }
}

// Set the direction of snake 2
template<typename T>
void Game<T>::setDirection2(int dx, int dy) {
    // Prevent the snake from reversing into itself
    if (dx != -direction2.first || dy != -direction2.second) {
        direction2 = {dx, dy};
    }
}

// Display the grid
template<typename T>
void Game<T>::displayGrid(bool& isFirstRender) {
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

template<typename T>
void Game<T>::start() {
    bool isFirstRender = true;  // Track if it's the first render

    Renderer::setupSignalHandler(grid.getHeight());

    auto lastMoveTime = std::chrono::steady_clock::now();

    // Game loop
    while (!this->getGameOver()) {
        // Handle input (track keypresses)
        handleInput();

        auto currentTime = std::chrono::steady_clock::now();
        std::chrono::duration<double, std::milli> timeSinceLastMove = currentTime - lastMoveTime;

        // Move the snakes based on the current snakeSpeed
        if (timeSinceLastMove.count() >= snakeSpeed) {
            this->update();  // Update the snakes' positions
            this->displayGrid(isFirstRender);  // Only update the specific parts of the grid that changed
            lastMoveTime = currentTime;  // Update last move time
        }

        // Small delay to avoid CPU overuse
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    // Display which player lost in multiplayer mode
    if (isMultiplayer) {
        std::cout << "Game Over! Player " << playerWhoLost << " lost!" << std::endl;
    } else {
        std::cout << "Game Over!" << std::endl;
    }
}

// Explicit template instantiation for char
template class Game<char>;
