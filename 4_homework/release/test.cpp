#include <iostream>
#include <fstream>
#include <string>

using namespace std;


int main() {
	ifstream fin, fin2;
	fin.open("outtmp.txt");
	fin2.open("example.txt");
	string eq, eq2;
	int counter = 1;
	while (getline(fin, eq)) {
		getline(fin2, eq2);
		if (eq != eq2) {
			std::cerr << "Difference in line " << counter << ":\n";
			std:cerr << "Right line: " << eq2 << endl;
			std::cerr << "Incorrect line" << eq << endl;
			return -1;
		}
		counter++;
	}
	std::cout << "OK" << endl;
	fin.close();
	fin2.close();
	return 0;

}
