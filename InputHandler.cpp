#include "InputHandler.h"
#include "Game.h"
#include <conio.h>

void InputHandler::handleInput(Game &game) {
    if (_kbhit()) {
        int key = _getch();
        if (key == 0 || key == 224) {
            key = _getch();
        }

        if(key != 'a' && key != 'd' && key != 75 && key != 77 && key != '1' && key != '2' && key != '3') {
          game.keys['a'] = false;
          game.keys['d'] = false;
          game.keys[75] = false;
          game.keys[77] = false;
        } else {
          game.keys[key] = true;
         }

        switch (key) {
            case 'w': case 'W': game.setDirection1(-1, 0); break;
            case 's': case 'S': game.setDirection1(1, 0); break;
            case 'a': case 'A': game.setDirection1(0, -1); break;
            case 'd': case 'D': game.setDirection1(0, 1); break;
            case 72: game.setDirection2(-1, 0); break;  // Up arrow
            case 80: game.setDirection2(1, 0); break;   // Down arrow
            case 75: game.setDirection2(0, -1); break;  // Left arrow
            case 77: game.setDirection2(0, 1); break;   // Right arrow
        }
    }
}
