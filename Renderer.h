#ifndef RENDERER_H
#define RENDERER_H

#include "Grid.h"
#include <iostream>
#include <windows.h>
#include <csignal>

inline int globalGridHeight = 0;

class Renderer {
public:
    static void setCursorPosition(short x, short y);
    static void clearScreen();
    static void hideCursor();
    static void moveCursorBelowGrid(int gridHeight);
    static void setupSignalHandler(int gridHeight);  // Setup signal handling

    template<class T>
    static void printEntireGrid(const Grid<T> &grid, HANDLE hConsole);

    template<typename T>
    static void printGrid(const Grid<T> &currentGrid, const Grid<T> &previousGrid, bool isFirstRender);

private:
    static void signalHandler(int signum);  // Static method for signal handling
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

inline void Renderer::hideCursor() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = false;  // Hide the cursor
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}

inline void Renderer::moveCursorBelowGrid(const int gridHeight) {
    setCursorPosition(0, gridHeight + 2);  // Move cursor two lines below the last grid row
}

inline void Renderer::signalHandler(int signum) {
    moveCursorBelowGrid(globalGridHeight);  // Use global variable for grid height

    // Restore cursor visibility before exiting
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = true;
    SetConsoleCursorInfo(hConsole, &cursorInfo);

    std::cout << "\nProgram interrupted with Ctrl+C" << std::endl;
    exit(signum);
}

inline void Renderer::setupSignalHandler(int gridHeight) {
    globalGridHeight = gridHeight;  // Set global grid height
    signal(SIGINT, signalHandler);  // Register the signal handler
}


template<typename T>
void Renderer::printEntireGrid(const Grid<T> &grid, HANDLE hConsole) {
    for (int i = 0; i < grid.getHeight(); ++i) {
        for (int j = 0; j < grid.getWidth(); ++j) {
            SetConsoleTextAttribute(hConsole, (grid.getCell(i, j) != grid.getDefaultValue()) ? 2 : 12);
            setCursorPosition(j * 2, i);  // Ensure each cell is printed at the correct position
            std::cout << grid.getCell(i, j);
        }
        std::cout << '\n';  // Move to the next line after each row
    }
}

template<typename T>
void Renderer::printGrid(const Grid<T> &currentGrid, const Grid<T> &previousGrid, bool isFirstRender) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    if (isFirstRender) {
        clearScreen();  // Clear the screen only once at the start
        setCursorPosition(0, 0);  // Move cursor to the top-left corner
        printEntireGrid(currentGrid, hConsole);  // Print the entire grid once
    } else {
        // Loop through the grid and update only the cells that changed
        for (int i = 0; i < currentGrid.getHeight(); ++i) {
            for (int j = 0; j < currentGrid.getWidth(); ++j) {
                T currentValue = currentGrid.getCell(i, j);
                T previousValue = previousGrid.getCell(i, j);

                // Only update if the value has changed
                if (currentValue != previousValue) {
                    setCursorPosition(j * 2, i);  // Set cursor to the correct position
                    SetConsoleTextAttribute(hConsole, (currentValue != currentGrid.getDefaultValue()) ? 2 : 12);
                    std::cout << currentValue;
                }
            }
        }
        std::cout.flush();  // Ensure output is flushed immediately
    }

    hideCursor();  // Hide the cursor after each render
}

#endif // RENDERER_H
