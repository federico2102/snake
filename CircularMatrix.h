#ifndef CIRCULARMATRIX_H
#define CIRCULARMATRIX_H

#include <vector>
#include <stdexcept>
#include <algorithm>

template<typename T>
class CircularMatrix {
    int width, height;
    std::vector<std::vector<T>> matrix;

public:
    CircularMatrix(int w, int h, T value);

    class CircularRow {
        std::vector<T>& row;
        const std::vector<T>& constRow;
        int height;

    public:
        CircularRow(std::vector<T>& r, int h);
        CircularRow(const std::vector<T>& r, int h);

        T& operator[](int j);
        const T& operator[](int j) const;
    };

    CircularRow operator[](int i);
    CircularRow operator[](int i) const;

    bool allValuesAreEqualTo(T value) const;
    bool operator==(const CircularMatrix &other) const;
};

template<typename T>
CircularMatrix<T>::CircularMatrix(int w, int h, T value) : width(w), height(h) {
    if (w <= 0 || h <= 0) {
        throw std::invalid_argument("Grid dimensions must be greater than zero.");
    }
    matrix = std::vector<std::vector<T>>(w, std::vector<T>(h, value));
}

template<typename T>
typename CircularMatrix<T>::CircularRow CircularMatrix<T>::operator[](int i) {
    i = (i % width + width) % width; // Circular access
    return CircularRow(matrix[i], height);
}

template<typename T>
typename CircularMatrix<T>::CircularRow CircularMatrix<T>::operator[](int i) const {
    i = (i % width + width) % width; // Circular access
    return CircularRow(matrix[i], height);
}

template<typename T>
CircularMatrix<T>::CircularRow::CircularRow(std::vector<T>& r, int h) : row(r), constRow(r), height(h) {}

template<typename T>
CircularMatrix<T>::CircularRow::CircularRow(const std::vector<T>& r, int h) : row(const_cast<std::vector<T>&>(r)), constRow(r), height(h) {}

template<typename T>
T& CircularMatrix<T>::CircularRow::operator[](int j) {
    j = (j % height + height) % height; // Circular access
    return row[j];
}

template<typename T>
const T& CircularMatrix<T>::CircularRow::operator[](int j) const {
    j = (j % height + height) % height; // Circular access
    return constRow[j];
}

template<typename T>
bool CircularMatrix<T>::allValuesAreEqualTo(T value) const {
    for (const auto& row : matrix) {
        if (std::any_of(row.begin(), row.end(), [=](T val) { return val != value; })) {
            return false;
        }
    }
    return true;
}

template<typename T>
bool CircularMatrix<T>::operator==(const CircularMatrix &other) const {
    return width == other.width && height == other.height && matrix == other.matrix;
}

#endif // CIRCULARMATRIX_H
