#include "Game.h"
#include <conio.h>  // For _kbhit() and _getch() in Windows
#include <iostream>
#include <chrono>
#include <thread>

int main() {
    // Symbols for snake, apple, and empty cells
    char snakeChar = 'S';
    char appleChar = 'A';
    char emptyChar = '.';

    // Create a game with a 20x20 grid and the chosen symbols
    Game<char> game(20, 20, snakeChar, appleChar, emptyChar);

    const int snakeSpeed = 200;     // Snake movement delay in milliseconds
    auto lastMoveTime = std::chrono::steady_clock::now();

    // Game loop
    while (!game.getGameOver()) {
        // Handle input (W, A, S, D)
        if (_kbhit()) {
            char key = _getch();  // Get the pressed key
            switch (key) {
                case 'w': game.setDirection(-1, 0); break;  // Move up
                case 's': game.setDirection(1, 0); break;   // Move down
                case 'a': game.setDirection(0, -1); break;  // Move left
                case 'd': game.setDirection(0, 1); break;   // Move right
                default: break;
            }
        }

        auto currentTime = std::chrono::steady_clock::now();
        std::chrono::duration<double, std::milli> timeSinceLastMove = currentTime - lastMoveTime;

        // Move the snake at a slower speed (e.g., every 200 milliseconds)
        if (timeSinceLastMove.count() >= snakeSpeed) {
            game.update();  // Update the snake's position
            game.displayGrid();  // Only update the specific parts of the grid that changed
            lastMoveTime = currentTime;  // Update last move time
        }

        // Small delay to avoid CPU overuse
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    std::cout << "Game Over!" << std::endl;
    return 0;
}
