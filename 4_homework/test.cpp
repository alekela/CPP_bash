#include <iostream>
#include <fstream>
#include <string>

using namespace std;


int main() {
	ifstream fin, fin2;
	fin.open("test.txt");
	fin2.open("example.txt");
	string eq, eq2;
	int counter = 1;
	while (getline(fin, eq)) {
		if (getline(fin2, eq2)){
			if (eq != eq2) {
				cerr << "Difference in line " << counter << ":\n";
				cerr << "Right line: " << eq2 << endl;
				cerr << "Incorrect line" << eq << endl;
				fin.close();
				fin2.close();
				return 0;
			}
			counter++;
		}
		else {
			cerr << "Different number of lines in files!" << endl;
			fin.close();
			fin2.close();
			return 0;
		}
	}
	if (getline(fin2, eq2)){
		cerr << "Different number of lines in files222222222222!" << endl;
		fin.close();
		fin2.close();
		return 0;
	}
	std::cout << "OK" << endl;
	fin.close();
	fin2.close();
	return 0;
}
