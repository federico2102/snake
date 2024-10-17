#include "Game.h"
#include <iostream>
#include <chrono>

int main() {
    int gridHeight = 20;
    int gridWidth = 20;
    char defaultSetup = ' ';

    char snakeChar1 = 'S';
    char snakeChar2 = 'T';
    char appleChar = 'A';
    char emptyChar = '.';
    bool isMultiplayer = false;

    std::cout << "Multiplayer mode? Y/N" << std::endl;
    char multiplayerOption;
    std::cin >> multiplayerOption;
    isMultiplayer = std::tolower(multiplayerOption) == 'y';

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
        std::cout << "Enter symbol for snake 1: " << std::endl;
        std::cin >> snakeChar1;
        if(isMultiplayer) {
            std::cout << "Enter symbol for snake 2: " << std::endl;
            std::cin >> snakeChar2;
        }
        std::cout << "Enter symbol for apple: " << std::endl;
        std::cin >> appleChar;
        std::cout << "Enter symbol for empty spaces: " << std::endl;
        std::cin >> emptyChar;
    }

    try {
        Game game(gridHeight, gridWidth, snakeChar1, snakeChar2, appleChar, emptyChar, isMultiplayer);
        game.start();
    } catch (const std::invalid_argument& e) {
        std::cerr << "Error: " << e.what() << '\n';
    }

    return 0;
}
