#include <iostream>
#include <iterator>
#include <cmath>
#include <vector>

using namespace std;


template <typename T>
class Matrix {
private:
    T** _values;
    int _size;

public:
    Matrix() {
        _size = 0;
    }

    Matrix(unsigned size) {
        if (size <= 0)
        {
            throw invalid_argument("Matrix dimensions can't be zero");
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

        for (int i = 0; i < _size + 1; i++)
            tmp0[i] = (T*) new T[_size + 1];

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


struct Vertex {
    int name;
    int place_in_matrix;
};

class Graph {
private:
    Matrix<int> _matrix;

    friend ostream& operator << (std::ostream& os, const Graph& graph)
    {
        return os << graph._matrix;
    }

public:
    Graph() {}

    Graph(const Graph& graph) {
        _matrix = graph._matrix;
    }

    ~Graph() {
        cout << "Bye-bye";
    }

    void add_vertex(initializer_list<int> input = {}) {
        if (_matrix.getSize() == 0) {
            _matrix.addElem();
        }
        else {
            _matrix.addElem();
            int counter = 0;
            int v, weight;
            for (int i : input) {
                if (counter % 2 == 0) {
                    v = i;
                    counter = counter == 0 ? 1 : 0;
                }
                else {
                    weight = i;
                    if (v == _matrix.getSize() - 1) {
                        throw exception();
                    }
                    _matrix.setValue(weight, v, _matrix.getSize() - 1);
                    _matrix.setValue(weight, _matrix.getSize() - 1, v);
                    counter = counter == 0 ? 1 : 0;
                }
            }
        }
    }

    void delete_vertex(int num) {
        _matrix.delElem(num);
    }

    void add_edge(int weight, int vertex1, int vertex2) {
        if (vertex1 == vertex2) {
            throw exception("There can't be edge between these vertexes");
        }
        _matrix.setValue(weight, vertex1, vertex2);
        _matrix.setValue(weight, vertex2, vertex1);

    }

    void delete_edge(int vertex1, int vertex2) {
        _matrix.setValue(0, vertex1, vertex2);
        _matrix.setValue(0, vertex1, vertex2);
    }
};


int main() {
    Graph graph;
    graph.add_vertex();
    graph.add_vertex({ 0, 10 });
    graph.add_vertex({ 0, 20, 1, 30 });
    cout << graph;
    delete& graph;
}
