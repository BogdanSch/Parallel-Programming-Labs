#pragma once
class SquareMatrix
{
private:
    int** data;
    bool isValidIndex(int row, int column) const {
        return (row >= 0 && row < size) &&
        (column >= 0 && column < size);
    }
public:
    size_t size;
    SquareMatrix(size_t n) : size(n) {
        data = new int*[size];
        for (size_t i = 0; i < size; i++) {
            data[i] = new int[size]{};
        }
    }
    ~SquareMatrix() {
        for (size_t i = 0; i < size; i++) {
            delete[] data[i];
        }
        delete[] data;
    }
    int get(int row, int column) const;
    void set(int row, int column, int value);
    void add(int row, int column, int value);
    SquareMatrix operator*(const SquareMatrix& other) const;
};