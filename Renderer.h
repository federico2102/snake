#ifndef RENDERER_H
#define RENDERER_H

#include "Grid.h"
#include <iostream>
#include <windows.h>

class Renderer {
public:
    static void setCursorPosition(short x, short y);
    static void clearScreen();

    template<typename T>
    static void printGrid(const ::Grid<T> &grid);
};

inline void Renderer::setCursorPosition(const short x, const short y) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    const COORD coord = {x, y};
    SetConsoleCursorPosition(hConsole, coord);
}

inline void Renderer::clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

template<typename T>
void Renderer::printGrid(const Grid<T> & grid) {
    clearScreen();
    setCursorPosition(0, 0); // Move cursor to the top-left corner

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    for (int i = 0; i < grid.getWidth(); i++) {
        for (int j = 0; j < grid.getHeight(); j++) {
            SetConsoleTextAttribute(hConsole, (grid.getCell(i,j) != grid.getDefaultValue()) ? 2 : 12);
            std::cout << grid.getCell(i,j) << "  ";
        }
        std::cout << '\n';
    }
    std::cout << '\n';
}

#endif // RENDERER_H

