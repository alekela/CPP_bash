#include <iostream>
#include <vector>
#include <string>

using namespace std;

int main(int argc, char* argv[]){
	vector<string> new_argv;
	for (int i = 1; i < argc; i++) {
		new_argv.push_back(argv[i]);
	}
	for (auto i: new_argv) {
		cout << i << endl;
	}
	return 0;
}
