#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int main(int argc, char* argv[]) {
	int n, tmp;
	vector<int> m;
	if (argv[1]) {
		ifstream file(argv[1]);
		file >> n;
		for (int i = 0; i < n; i++) {
			file >> tmp;
			m.push_back(tmp);
		}
		file.close();

	}
	else {
		cin >> n;
		for (int i = 0; i < n; i++) {
			cin >> tmp;
			m.push_back(tmp);
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
}
