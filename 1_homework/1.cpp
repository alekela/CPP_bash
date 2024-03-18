#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int main(int argc, char* argv[]) {
	int n, tmp;
	vector<int> m;
	if (argv[1]) {
		ifstream file(argv[1]);
		if (file) {
			file >> n;
			for (int i = 0; i < n; i++) {
				file >> tmp;
				m.push_back(tmp);
			}
		}
		else {
			cerr << "Incorrect file name!" << endl;
		}
		file.close();

	}
	else {
		cout << "Enter amount of numbers:\t";
		cin >> n;
		if (n <= 0){
			cerr << "Incorrect amount of numbers!" << endl;
		}
		else{
			cout << "Enter numbers in line:\n";
			for (int i = 0; i < n; i++) {
				cin >> tmp;
				m.push_back(tmp);
			}
		}
	}

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n - 1 - i; j++) {
			if (m[j] > m[j + 1]) {
				swap(m[j], m[j + 1]);
			}
		}
	}

	for (int i = 0; i < n; i++) {
		cout << m[i] << endl;
	}
	ofstream outfile("output.txt");
	for (int i = 0; i < n; i++) {
		outfile << m[i] << endl;
	}
	outfile.close();
}
