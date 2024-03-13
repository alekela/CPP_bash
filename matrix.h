#ifndef MATRIX
#define MATRIX

#include <iostream>
#include <iterator>
#include <cmath>
#include <vector>
#include <iomanip>


class Matrix {
private:
    int** _values;
    int _size;

public:
    Matrix(unsigned size);

    Matrix(const Matrix& _matrix);


    ~Matrix();

    friend std::ostream& operator<<(std::ostream& os, const Matrix& matrix);

    Matrix& operator=(const Matrix& matrix);

    int& getValue(const int i, const int j) {
        return this->_values[i][j];
    }

    void setValue(int value, int i, int j) {
        _values[i][j] = value;
    }

    int getSize() { return _size; };

    void addElem();

    void delElem(int num);
};

#endif
