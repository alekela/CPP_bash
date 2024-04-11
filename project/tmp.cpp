#include <fstream>
#include <string>
#include <iostream>

using namespace std;

int main(){
	ifstream fin;
	ofstream fout;
	fin.open("level1.txt");
	fout.open("level2.txt");
	string tmp;
	while(getline(fin, tmp)) {
		for (auto i : tmp) {
			if (i != ' ') {
				fout << i;
			}
		}
		fout << "\n";
	}
	fin.close();
	fout.close();
}
