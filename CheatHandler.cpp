#include "CheatHandler.h"
#include "Game.h"

void CheatHandler::checkForCheats(Game &game, std::list<std::pair<int, int>> &snake1, std::list<std::pair<int, int>> &snake2) {
    if (game.keys['a'] && game.keys['d']) {
        growSnake(snake1, 3);
        game.keys['a'] = false;
        game.keys['d'] = false;
    }

    if (game.keys[75] && game.keys[77]) {
        growSnake(snake2, 3);
        game.keys[75] = false;
        game.keys[77] = false;
    }

    if (game.keys['1'] && game.keys['2'] && game.keys['3']) {
        activateParadiseMode(game);
        game.keys['1'] = false;
        game.keys['2'] = false;
        game.keys['3'] = false;
    }
}

// Grow the snake by a certain number of segments
void CheatHandler::growSnake(std::list<std::pair<int, int>>& snake, const int growth) {
    auto tail = snake.back(); // Capture the tail before growing
    for (int i = 0; i < growth; ++i) {
        snake.push_back(tail); // Add new segments at the tail position
    }
}

// Activate paradise mode (lots of apples everywhere, limited to 1/8 of the grid)
void CheatHandler::activateParadiseMode(Game &game) {
    game.paradiseModeActive = true;

    // Limit apples to 1/8 of the grid size
    const int maxApples = (game.grid.getWidth() * game.grid.getHeight()) / 8;
    game.generateFoodPositions(maxApples);
}