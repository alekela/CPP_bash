#include "matrix.h"


Matrix::Matrix(unsigned size) {
    if (size < 0)
    {
        throw std::invalid_argument("Matrix dimensions can't be negative");
    }

    _size = size;

    _values = (int**) new int * [_size];

    for (int i = 0; i < _size; i++)
        _values[i] = (int*) new int[_size];

    for (int i = 0; i < _size; i++) {
        for (int j = 0; j < _size; ++j) {
            _values[i][j] = 0;
        }
    }

}

Matrix::Matrix(const Matrix& _matrix) {
    _size = _matrix._size;

    _values = (int**) new int * [_size];

    for (int i = 0; i < _size; i++) {
        _values[i] = (int*) new int[_size];
    }
    for (int i = 0; i < _size; i++) {
        for (int j = 0; j < _size; j++) {
            _values[i][j] = _matrix._values[i][j];
        }
    }
}

Matrix::~Matrix() {
    for (int i = 0; i < _size; i++) {
        delete[] _values[i];
    }
    delete[] _values;
}

std::ostream& operator<<(std::ostream& os, const Matrix& matrix) {
    os << "      ";
    for (int i = 0; i < matrix._size; i++) {
        os << std::setw(4) << i;
    }
    os << '\n';
    for (int i = 0; i < matrix._size; i++) {
        os << std::setw(3) << i << ": ";
        os << '[';
        for (int j = 0; j < matrix._size; j++) {
            os << std::setw(4) << matrix._values[i][j];
        }
        os << "]\n";
    }
    os << '\n';
    return os;
}

Matrix& Matrix::operator=(const Matrix& matrix) {
    if (this == &matrix) {
        return *this;
    }
    if (_size != matrix._size) {
        for (int i = 0; i < _size; i++)
            delete[] _values[i];
        delete[] _values;

        _size = matrix._size;

        _values = (int**) new int * [matrix._size];
        for (int i = 0; i < matrix._size; i++) {
            _values[i] = (int*) new int[matrix._size];
        }
    }

    for (int i = 0; i < _size; i++) {
        for (int j = 0; j < _size; j++) {
            _values[i][j] = matrix._values[i][j];
        }
    }
    return *this;
}

void Matrix::addElem() {
    int** tmp0;
    tmp0 = (int**) new int * [_size + 1];

    for (int i = 0; i < _size + 1; i++) {
        tmp0[i] = (int*) new int[_size + 1];
    }

    for (int i = 0; i < _size; i++) {
        for (int j = 0; j < _size; j++) {
            tmp0[i][j] = _values[i][j];
        }
    }
    for (int i = 0; i < _size + 1; i++) {
        tmp0[i][_size] = 0;
        tmp0[_size][i] = 0;
    }


    for (int i = 0; i < _size; i++) {
        delete[] _values[i];
    }
    delete[] _values;

    _values = tmp0;

    _size++;
}

void Matrix::delElem(int num) {
    int** tmp0;
    tmp0 = (int**) new int * [_size - 1];

    for (int i = 0; i < _size - 1; i++)
        tmp0[i] = (int*) new int[_size - 1];

    for (int i = 0; i < _size; i++) {
        for (int j = 0; j < _size; j++) {
            if (i != num && j != num) {
                if (i > num && j > num) {
                    tmp0[i - 1][j - 1] = _values[i][j];
                }
                else if (i > num) {
                    tmp0[i - 1][j] = _values[i][j];
                }
                else if (j > num) {
                    tmp0[i][j - 1] = _values[i][j];
                }
                else {
                    tmp0[i][j] = _values[i][j];
                }
            }
        }
    }

    for (int i = 0; i < _size; i++) {
        delete[] _values[i];
    }
    delete[] _values;

    _values = tmp0;

    _size--;
}