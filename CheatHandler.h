#ifndef CHEAT_HANDLER_H
#define CHEAT_HANDLER_H

#include <list>
#include <utility>

class Game;
class CheatHandler {
public:
    static void checkForCheats(Game &game, std::list<std::pair<int, int>> &snake1, std::list<std::pair<int, int>> &snake2);
    static void growSnake(std::list<std::pair<int, int>>& snake, int growth); // Function to grow a snake
    static void activateParadiseMode(Game &game); // Activate paradise mode
};

#endif // CHEAT_HANDLER_H
