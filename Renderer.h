#ifndef RENDERER_H
#define RENDERER_H

#include "Grid.h"
#include <iostream>
#include <csignal>

#ifdef _WIN32
#include <windows.h>
#endif

inline int globalGridHeight = 0;

class Renderer {
public:
    static void setCursorPosition(short x, short y);
    static void clearScreen();
    static void hideCursor();
    static void moveCursorBelowGrid(int gridHeight);
    static void setupSignalHandler(int gridHeight);  // Setup signal handling

    template<class T>
    static void printEntireGrid(const Grid<T> &grid);

    template<typename T>
    static void printGrid(const Grid<T> &currentGrid, const Grid<T> &previousGrid, bool isFirstRender);

private:
    static void setTextColor(bool isActiveCell);  // Sets color based on cell state
    static void signalHandler(int signum);  // Static method for signal handling
};

#ifdef _WIN32

inline void Renderer::setCursorPosition(const short x, const short y) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    const COORD coord = {x, y};
    SetConsoleCursorPosition(hConsole, coord);
}

inline void Renderer::clearScreen() {
    system("cls");
}

inline void Renderer::hideCursor() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}

inline void Renderer::moveCursorBelowGrid(const int gridHeight) {
    setCursorPosition(0, gridHeight + 2);  // Move cursor two lines below the last grid row
}

inline void Renderer::setTextColor(bool isActiveCell) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, isActiveCell ? 2 : 12);  // Green for active, red for inactive
}

#else  // macOS/Linux

inline void Renderer::setCursorPosition(const short x, const short y) {
    std::cout << "\033[" << y + 1 << ";" << x + 1 << "H";  // ANSI escape code for setting cursor position
}

inline void Renderer::clearScreen() {
    std::cout << "\033[2J\033[H";  // ANSI escape code to clear screen and move cursor to top-left
}

inline void Renderer::hideCursor() {
    std::cout << "\033[?25l";  // ANSI escape code to hide cursor
}

inline void Renderer::moveCursorBelowGrid(const int gridHeight) {
    setCursorPosition(0, gridHeight + 2);  // Move cursor two lines below the last grid row
}

inline void Renderer::setTextColor(bool isActiveCell) {
    if (isActiveCell) {
        std::cout << "\033[32m";  // Green for active cells
    } else {
        std::cout << "\033[31m";  // Red for inactive cells
    }
}

#endif  // _WIN32

inline void Renderer::signalHandler(int signum) {
    moveCursorBelowGrid(globalGridHeight);  // Use global variable for grid height

    // Restore cursor visibility before exiting
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = true;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
#else
    std::cout << "\033[?25h";  // ANSI escape code to show cursor on UNIX-based systems
#endif

    std::cout << "\nProgram interrupted with Ctrl+C" << std::endl;
    exit(signum);
}

inline void Renderer::setupSignalHandler(int gridHeight) {
    globalGridHeight = gridHeight;  // Set global grid height
    signal(SIGINT, signalHandler);  // Register the signal handler
}

template<typename T>
void Renderer::printEntireGrid(const Grid<T> &grid) {
    for (int i = 0; i < grid.getHeight(); ++i) {
        for (int j = 0; j < grid.getWidth(); ++j) {
            bool isActiveCell = (grid.getCell(i, j) != grid.getDefaultValue());
            setTextColor(isActiveCell);  // Set color based on cell state
            setCursorPosition(j * 2, i);  // Ensure each cell is printed at the correct position
            std::cout << grid.getCell(i, j);
        }
        std::cout << '\n';  // Move to the next line after each row
    }
    std::cout.flush();  // Ensure output is flushed
}

template<typename T>
void Renderer::printGrid(const Grid<T> &currentGrid, const Grid<T> &previousGrid, bool isFirstRender) {
    if (isFirstRender) {
        clearScreen();  // Clear the screen only once at the start
        setCursorPosition(0, 0);  // Move cursor to the top-left corner
        printEntireGrid(currentGrid);  // Print the entire grid once
    } else {
        // Loop through the grid and update only the cells that changed
        for (int i = 0; i < currentGrid.getHeight(); ++i) {
            for (int j = 0; j < currentGrid.getWidth(); ++j) {
                T currentValue = currentGrid.getCell(i, j);
                T previousValue = previousGrid.getCell(i, j);

                // Only update if the value has changed
                if (currentValue != previousValue) {
                    setCursorPosition(j * 2, i);  // Set cursor to the correct position
                    bool isActiveCell = (currentValue != currentGrid.getDefaultValue());
                    setTextColor(isActiveCell);  // Set color based on cell state
                    std::cout << currentValue;
                }
            }
        }
        std::cout.flush();  // Ensure output is flushed immediately
    }

    hideCursor();  // Hide the cursor after each render
}

#endif  // RENDERER_H
