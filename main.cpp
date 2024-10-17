#include "Game.h"
#include <iostream>
#include <chrono>

int main() {
    int gridHeight = 20;
    int gridWidth = 20;
    char defaultSetup = ' ';
    bool isMultiplayer = false;

    // Symbols for snake 1, snake 2, apple, and empty cells
    char snakeChar1 = 'S';
    char snakeChar2 = 'Z';  // Second snake symbol
    char appleChar = 'A';
    char emptyChar = '.';

    // Ask if the user wants to play single-player or multiplayer
    char gameMode = ' ';
    while (gameMode != 's' && gameMode != 'm') {
        std::cout << "Choose game mode: (S)ingle-player or (M)ultiplayer" << std::endl;
        std::cin >> gameMode;
        gameMode = std::tolower(gameMode);
    }

    while (defaultSetup != 'y' && defaultSetup != 'n') {
        std::cout << "Do you want to use default settings for grid size and symbols? Y/N" << std::endl;
        std::cin >> defaultSetup;
        std::tolower(defaultSetup);
    }

    if (defaultSetup == 'n') {
        std::cout << "Enter grid width: " << std::endl;
        std::cin >> gridWidth;
        std::cout << "Enter grid height: " << std::endl;
        std::cin >> gridHeight;
        std::cout << "Enter symbol for snake 1" << std::endl;
        std::cin >> snakeChar1;
        if(gameMode == 'm') {
            std::cout << "Enter symbol for snake 2" << std::endl;
            std::cin >> snakeChar2;
        }
        std::cout << "Enter symbol for apple" << std::endl;
        std::cin >> appleChar;
        std::cout << "Enter symbol for empty spaces" << std::endl;
        std::cin >> emptyChar;
    }

    if (gameMode == 'm') {
        isMultiplayer = true;
    }

    try {
        // Create a game with a grid and the chosen symbols
        Game<char> game(gridHeight, gridWidth, snakeChar1, snakeChar2, appleChar, emptyChar, isMultiplayer);
        game.start();
    } catch (const std::invalid_argument& e) {
        std::cerr << "Error: " << e.what() << '\n';
    }
    return 0;
}
