#include "Game.h"
#include <iostream>
#include <chrono>

int main() {
    int gridHeight = 20;
    int gridWidth = 20;
    char defaultSetup = ' ';

    // Symbols for snake, apple, and empty cells
    char snakeChar = 'S';
    char appleChar = 'A';
    char emptyChar = '.';

    while (defaultSetup != 'y' && defaultSetup != 'n') {
        std::cout << "Do you want to use default settings for grid size and symbols? Y/N" << std::endl;
        std::cin >> defaultSetup;
        std::tolower(defaultSetup);
    }

    if (defaultSetup == 'N') {
        std::cout << "Enter grid width: " << std::endl;
        std::cin >> gridWidth;
        std::cout << "Enter grid height: " << std::endl;
        std::cin >> gridHeight;
        std::cout << "Enter symbol for snake" << std::endl;
        std::cin >> snakeChar;
        std::cout << "Enter symbol for apple" << std::endl;
        std::cin >> appleChar;
        std::cout << "Enter symbol for empty spaces" << std::endl;
        std::cin >> emptyChar;
    }

    try {
        // Create a game with a 20x20 grid and the chosen symbols
        Game<char> game(gridHeight, gridWidth, snakeChar, appleChar, emptyChar);
        game.start();
    } catch (const std::invalid_argument& e) {
        std::cerr << "Error: " << e.what() << '\n';
    }
    return 0;
}
