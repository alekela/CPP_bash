#ifndef MATRIX
#define MATRIX

#include <iostream>
#include <iterator>
#include <cmath>
#include <vector>


template <typename T>
class Matrix {
private:
    T** _values;
    int _size;

public:
    Matrix(unsigned size) {
        if (size < 0)
        {
            throw std::invalid_argument("Matrix dimensions can't be negative");
        }

        _size = size;

        _values = (T**) new T * [_size];

        for (int i = 0; i < _size; i++)
            _values[i] = (T*) new T[_size];

        for (int i = 0; i < _size; i++) {
            for (int j = 0; j < _size; ++j) {
                _values[i][j] = 0;
            }
        }

    }

    Matrix(const Matrix<T>& _matrix) {
        _size = _matrix._size;

        _values = (T**) new T * [_size];

        for (int i = 0; i < _size; i++) {
            _values[i] = (T*) new T[_size];
        }
        for (int i = 0; i < _size; i++) {
            for (int j = 0; j < _size; j++) {
                _values[i][j] = _matrix._values[i][j];
            }
        }
    }


    ~Matrix() {
        for (int i = 0; i < _size; i++) {
            delete[] _values[i];
        }
        delete[] _values;
    }

    template <typename T1>
    friend std::ostream& operator<<(std::ostream& os, const Matrix<T1>& matrix) {
        for (int i = 0; i < matrix._size; i++) {
            os << '[';
            for (int j = 0; j < matrix._size; j++) {
                os << matrix._values[i][j];
                if (j + 1 != matrix._size) {
                    os << '\t';
                }
            }
            os << "]\n";
        }
        return os;
    }

    Matrix<T>& operator=(const Matrix<T>& matrix) {
        if (this == &matrix){
            return *this;
        }
        if (_size != matrix._size) {
            for (int i = 0; i < _size; i++)
                delete[] _values[i];
            delete[] _values;

            _size = matrix._size;

            _values = (T**) new T * [matrix._size];
            for (int i = 0; i < matrix._size; i++) {
                _values[i] = (T*) new T[matrix._size];
            }
        }

        for (int i = 0; i < _size; i++) {
            for (int j = 0; j < _size; j++) {
                _values[i][j] = matrix._values[i][j];
            }
        }
        return *this;
    }

    T& getValue(const int i, const int j) {
        return this->_values[i][j];
    }

    const T& getValue(const int i, const int j) const {
        return this->_values[i][j];
    }

    void setValue(T value, int i, int j) {
        _values[i][j] = value;
    }

    int getSize() { return _size; };

    void addElem() {
        T** tmp0;
        tmp0 = (T**) new T * [_size + 1];

        for (int i = 0; i < _size + 1; i++) {
            tmp0[i] = (T*) new T[_size + 1];
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

    void delElem(int num) {
        T** tmp0;
        tmp0 = (T**) new T * [_size - 1];

        for (int i = 0; i < _size - 1; i++)
            tmp0[i] = (T*) new T[_size - 1];

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
};

#endif
