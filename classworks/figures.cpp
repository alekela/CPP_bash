#include <iostream>

using namespace std;

enum E_COLOR { E_COLOR_RED = 0, E_COLOR_GREEN = 1, E_COLOR_BLUE = 2 };

const char* STR_COLORS[3] = {"Red", "Green", "Blue"};

struct CPnt {
	int x;
	int y;

	CPnt(const int _x = 0, const int _y = 0) : x(_x), y(_y) {}
	CPnt(const CPnt& _cpy) : x(_cpy.x), y(_cpy.y) {}
	CPnt& operator=(const CPnt& _cpy) { x = _cpy.x; y=_cpy.y; }
};

ostream& operator<<(ostream& _os, const CPnt& _pnt) {
	_os << "(" << _pnt.x << "," << _pnt.y << ")";
	return _os;
}

class CColored {
public:
	E_COLOR color;

	CColored(const E_COLOR _clr) : color(_clr) {}
	virtual void printColor() const final { cout << STR_COLORS[color] << " "; }
};

class CFigure {
protected:	
	string name;

	CFigure( const char* _name) : name(_name) {}
	void printName() const { cout << name << ": "; }

public:
	virtual void print() const = 0;
	virtual ~CFigure() {} 
};

class CPolygon : public CFigure {
	friend ostream& operator<<(ostream& _os, const CPolygon& _poly);
private:

protected:	
	size_t n;
	CPnt*  angles;

public:
	CPolygon( const size_t _n, const CPnt* const _angles) : n (_n), CFigure("Polygon") {
		if (n == 0 || _angles == nullptr) {
			// Some error processing
		}
		angles = new CPnt[n];
		for (size_t i = 0; i < n; i++) {
			angles[i] = _angles[i];
		}
	}
	
	CPolygon(const CPolygon& _cpy) = delete;

	~CPolygon() {
		if (n != 0 && angles != nullptr) {
			delete [] angles;
		}
	}

	unsigned int getN() const { return n; }
	CPnt& operator[](const size_t _idx) { 
		if (_idx > n) {
			// Some error processing
		}

		return angles[_idx];
	}

	void print() const override {
		printName();
		for (size_t i = 0; i < n; i++) {
			cout << angles[i];
		}
		cout << endl;
	}
};

class CColoredPolygon : public CPolygon, public CColored {
	friend ostream& operator<<(ostream& _os, const CColoredPolygon& _poly);
public:
	CColoredPolygon(const E_COLOR _clr, const size_t _n, const CPnt* const _angles) : CColored(_clr), CPolygon(_n, _angles) {}
	
	CColoredPolygon(const CPolygon& _cpy) = delete;

	void print() const override {
		printColor();
		CPolygon::print();
	}
};

ostream& operator<<(ostream& _os, const CPolygon& _poly) {
	_os << _poly.name;
	for (size_t i = 0; i < _poly.n; i++) {
		_os << _poly.angles[i];
	}
	return _os;
}

ostream& operator<<(ostream& _os, const CColoredPolygon& _poly) {
	_os << STR_COLORS[_poly.color] << " " << _poly.name;
	for (size_t i = 0; i < _poly.n; i++) {
		_os << _poly.angles[i];
	}
	return _os;
}

struct CCircle : public CFigure {
	CPnt center;
	unsigned int r;

	CCircle( const CPnt& _c, const unsigned int _r) : center(_c), r(_r), CFigure("Circle") {}
	void print() const override {
		printName();
		cout << center << ", R = " << r << endl;

	}
};

struct CColoredCircle : public CCircle, public CColored {
	CColoredCircle(const E_COLOR _clr, const CPnt& _c, const unsigned int _r) : CColored(_clr), CCircle(_c, _r) {}
	void print() const override {
		printColor();
		CCircle::print();
	}
};	

struct CTriangle : public CPolygon {
	CTriangle( const CPnt* const _angles) : CPolygon(3, _angles) {
		name = "Triangle";
	};
};

struct CQuadrangle : public CPolygon {
	CQuadrangle(const CPnt* const _angles) : CPolygon(4, _angles) {
		name = "Quadrangle";
	};
};

struct CPentangle : public CPolygon {
	CPentangle(const CPnt* const _angles) : CPolygon(5, _angles) {
		name = "Pentangle";
	};
};

struct CColoredTriangle : public CColoredPolygon {
	CColoredTriangle(const E_COLOR _clr, const CPnt* const _angles) : CColoredPolygon(_clr, 3, _angles) {
		name = "Triangle";
	};
};

struct CColoredQuadrangle : public CColoredPolygon {
	CColoredQuadrangle(const E_COLOR _clr, const CPnt* const _angles) : CColoredPolygon(_clr, 4, _angles) {
		name = "Quadrangle";
	};
};

struct CColoredPentangle : public CColoredPolygon {
	CColoredPentangle(const E_COLOR _clr, const CPnt* const _angles) : CColoredPolygon(_clr, 5, _angles) {
		name = "Pentangle";
	};
};


int main(int argc, char* argv[]) {
	CPnt pnts[5] = {{0, 0}, {5, 0}, {5, 5}, {10,10}, {0, 5}};

	CColoredTriangle theTriangle(E_COLOR_RED, pnts);
	CQuadrangle theQuadrangle(pnts);
	CColoredPentangle thePentangle(E_COLOR_BLUE, pnts);
	CCircle theCircle(CPnt(2,2), 3);

	CFigure* figures[4];
	figures[0] = &theTriangle;
	figures[1] = &theQuadrangle;
	figures[2] = &thePentangle;
	figures[3] = &theCircle;
//	figures[0] = new CTriangle(E_COLOR_RED, pnts);
//	figures[1] = new CQuadrangle(E_COLOR_GREEN, pnts);
//	figures[2] = new CPentangle(E_COLOR_BLUE, pnts);
//	figures[3] = new CCircle(E_COLOR_RED, CPnt(2,2), 3);


	for (size_t i = 0; i < 4; i++) {
		figures[i]->print();
	}

	cout << thePentangle << endl;

//	theQuadrangle[2] = CPnt(100, 100);
//	theQuadrangle.print();

//	CPolygon tempPoly = theTriangle;


//	for (size_t i = 0; i < 4; i++) {
//		delete figures[i];
//	}

	


	return 0;
}
