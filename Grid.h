#ifndef GRID_H
#define GRID_H

#include "CircularMatrix.h"
#include <set>
#include <utility>

template<typename T>
class Grid {
private:
    int width;
    int height;
    T defaultValue;
    CircularMatrix<T> matrix;
    std::set<std::pair<int, int>> activeCells;  // Track active cells (alive cells and their neighbors)

public:
    Grid(int w, int h, T d);
    T getCell(int x, int y) const;
    void setCell(int x, int y, T value);
    int getWidth() const;
    int getHeight() const;
    std::set<std::pair<int, int>> getActiveCells() const;
    void setActiveCells(std::set<std::pair<int, int>> ac);
    T getDefaultValue() const;

    bool operator==(const Grid &other) const;
    bool allValuesAreEqualTo(T value) const;

};

template<typename T>
Grid<T>::Grid(const int w, const int h, const T d) : width(w), height(h), defaultValue(d), matrix(w, h, d) {}

template<typename T>
T Grid<T>::getCell(const int x,const int y) const {
    return matrix[x][y];
}

template<typename T>
void Grid<T>::setCell(const int x, const int y, const T value) {
    matrix[x][y] = value;
    if (value != defaultValue) {
        activeCells.insert({x, y});  // Track this cell as active
    } else {
        activeCells.erase({x, y});  // If cell dies, remove it from active cells
    }
}

template<typename T>
int Grid<T>::getWidth() const { return width; }

template<typename T>
int Grid<T>::getHeight() const { return height; }

template<typename T>
std::set<std::pair<int, int>> Grid<T>::getActiveCells() const { return activeCells; }

template<typename T>
void Grid<T>::setActiveCells(std::set<std::pair<int, int> > ac) {
    activeCells = std::move(ac);
}

template<typename T>
T Grid<T>::getDefaultValue() const {
    return defaultValue;
}


template<typename T>
bool Grid<T>::operator==(const Grid &other) const {
    return width == other.width && height == other.height && matrix == other.matrix;
}

template<typename T>
bool Grid<T>::allValuesAreEqualTo(const T value) const {
    return matrix.allValuesAreEqualTo(value);
}

#endif // GRID_H

