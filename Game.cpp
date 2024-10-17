#include "Game.h"
#include "InputHandler.h"
#include "CheatHandler.h"
#include <conio.h>
#include "Renderer.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <thread>

// Constructor
Game::Game(const int width, const int height, const char snakeSymbol1, const char snakeSymbol2, const char appleSymbol,
           const char emptySymbol, const bool isMultiplayer)
    : grid(width, height, emptySymbol), previousGrid(width, height, emptySymbol),
      direction1({0, 1}), direction2({0, -1}), isGameOver(false),
      isMultiplayer(isMultiplayer), applesEaten(0), snakeSpeed(200), playerWhoLost(0),
      snakeSymbol1(snakeSymbol1), snakeSymbol2(snakeSymbol2), appleSymbol(appleSymbol), emptySymbol(emptySymbol), maxApplesOnMap(1), paradiseModeActive(false) {
    initializeGame();

    // Initialize key states to false
    std::fill(std::begin(keys), std::end(keys), false);
}

// Initialize game elements
void Game::initializeGame() {
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
void Game::generateFoodPositions(const int numApples) {
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

// Move the snake in the current direction
void Game::moveSnake(std::list<std::pair<int, int>>& snake, const std::pair<int, int>& direction,
    const char snakeSymbol, const int playerNumber) {
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

bool Game::checkCollision(const std::pair<int, int>& newHead, const std::list<std::pair<int, int>>& snake) {
    for (const auto& segment : snake) {
        if (newHead == segment) {
            return true; // Collision detected
        }
    }
    return false;
}

// Update game state (move snakes)
void Game::update() {
    if (!isGameOver) {
        previousGrid = grid; // Store the previous state of the grid before updating

        // Move snake 1
        moveSnake(snake1, direction1, snakeSymbol1, 1); // Player 1 is snake 1

        // Move snake 2 if multiplayer and game is not over
        if (isMultiplayer) {
            moveSnake(snake2, direction2, snakeSymbol2, 2); // Player 2 is snake 2
        }

        // Check for cheat codes
        CheatHandler::checkForCheats(*this, snake1, snake2);
    }
}

// Set the direction of snake 1
void Game::setDirection1(int dx, int dy) {
    // Prevent the snake from reversing into itself
    if (dx != -direction1.first || dy != -direction1.second) {
        direction1 = {dx, dy};
    }
}

// Set the direction of snake 2
void Game::setDirection2(int dx, int dy) {
    // Prevent the snake from reversing into itself
    if (dx != -direction2.first || dy != -direction2.second) {
        direction2 = {dx, dy};
    }
}

// Display the grid
void Game::displayGrid(bool& isFirstRender) {
    Renderer::printGrid(grid, previousGrid, isFirstRender);  // The renderer will display the grid with the chosen symbols
    isFirstRender = false;
    previousGrid = grid;
}

// Get the game-over state
bool Game::getGameOver() const {
    Renderer::moveCursorBelowGrid(grid.getHeight());  // Move cursor two lines below the grid
    return isGameOver;
}

void Game::start() {
    bool isFirstRender = true;  // Track if it's the first render

    Renderer::setupSignalHandler(grid.getHeight());

    auto lastMoveTime = std::chrono::steady_clock::now();

    // Game loop
    while (!this->getGameOver()) {
        // Handle input (track keypresses)
        InputHandler::handleInput(*this);

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