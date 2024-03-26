#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include <cmath>
#include <fstream>

using namespace std;
typedef tuple <int, int, char> iic;


bool check_brackets(string eq) {
	int counter = 0;
	for (char i : eq) {
		if (i == '(') {
			counter++;
		}
		else if (i == ')') {
			counter--;
		}
	}
	return (counter == 0);
}


template <typename T>
T calc(string eq, bool details) {
	if (not check_brackets(eq)) {
		std::cerr << "Incorrect equation!";
		return 0;
	}
	vector<iic> signs;
	vector<T> numbers;
	string tmp_num;
	T tmp_num2;
	int bracket_counter = 0;
	int index_counter = 0;
	for (int i = 0; i < eq.length(); i++) {
		if (isdigit(eq[i]) || eq[i] == '.' || (i == 0 && eq[i] == '-') || (i != 0 && eq[i] == '-' && eq[i - 1] == '(')) {
			tmp_num.push_back(eq[i]);
		}
		else if (eq[i] == '(') {
			bracket_counter += 3;
		}
		else if (eq[i] == ')') {
			bracket_counter -= 3;
		}
		else if (eq[i] == '+' || eq[i] == '-' || eq[i] == '/' || eq[i] == '*' || eq[i] == '^') {
			tmp_num2 = stof(tmp_num);
			tmp_num.erase();
			numbers.push_back(tmp_num2);

			int tmp;
			if (eq[i] == '+' || eq[i] == '-') {
				tmp = 0;
			}
			else if (eq[i] == '/' || eq[i] == '*') {
				tmp = 1;
			}
			else if (eq[i] == '^') {
				tmp = 2;
			}
			iic t(bracket_counter + tmp, index_counter++, eq[i]);
			signs.push_back(t);
		}
	}
	tmp_num2 = stof(tmp_num);
	tmp_num.erase();
	numbers.push_back(tmp_num2);

	for (int i = 0; i < signs.size(); i++) { // bubble sort, can be modified
		for (int j = 0; j < signs.size() - i - 1; j++) {
			if (get<0>(signs[j]) < get<0>(signs[j + 1])) {
				swap(signs[j], signs[j + 1]);
			}
		}
	}

	//for (int i = 0; i < signs.size(); i++) 
	//	std::cout << get<0>(signs[i]) << " " << get<1>(signs[i]) << " " << get<2>(signs[i]) << endl;

	int index;
	for (int i = 0; i < signs.size(); i++) {
		index = get<1>(signs[i]);

		if (details) { // check if user wants more detailed info (flag -v)
			std::cout << numbers[index] << ' ' << get<2>(signs[i]) << ' ' << numbers[index + 1] << " = ";
		}
		if (get<2>(signs[i]) == '+') {
			numbers[index] = numbers[index] + numbers[index + 1];
		}
		else if (get<2>(signs[i]) == '-') {
			numbers[index] = numbers[index] - numbers[index + 1];
		}
		else if (get<2>(signs[i]) == '*') {
			numbers[index] = numbers[index] * numbers[index + 1];
		}
		else if (get<2>(signs[i]) == '/') {
			numbers[index] = numbers[index] / numbers[index + 1];
		}
		else if (get<2>(signs[i]) == '^') {
			numbers[index] = pow(numbers[index], numbers[index + 1]);
		}

		if (details) { // check if user wants more detailed info (flag -v)
			std::cout << numbers[index] << std::endl;
		}

		for (int j = i + 1; j < signs.size(); j++) { // deleting from numbers so changing indexes in signs
			if (get<1>(signs[j]) > index) {
				get<1>(signs[j])--;
			}
		}
		numbers.erase(numbers.begin() + index + 1); // deleting number that we used
	}
	return numbers[0];
}


int check_cmd_line_flags(int argc, char* argv[], const char* str) {
	for (int i = 1; i < argc; i++) {
		if (argv[i] == str) {
			return i;
		}
	}
	return 0;
}


int main() {
	int argc = 2;
	char* argv[] = {" ", "1+2", "2+3"};
	bool details;
	string eq;
	string outfilename = "";
	string infilename = "";
	if (check_cmd_line_flags(argc, argv, "-h")) {
		// cout help info
		return 0;
	}
	if (check_cmd_line_flags(argc, argv, "-v")) {
		details = true;
	}
	int tmp_index = check_cmd_line_flags(argc, argv, "-o");
	if (tmp_index) {
		if (tmp_index + 1 >= argc || (! argv[tmp_index + 1])) {
			std::cerr << "No output file name!";
			return -1;
		}
		outfilename = argv[tmp_index + 1];
	}
	if (argc == 2 && isalpha(argv[1][0])) {
		infilename = argv[1];
	}


	if (!infilename.empty()) { // reading from file
		ifstream fin;
		fin.open(infilename);
		if (!fin.is_open()) {
			std::cerr << "Can't open the input file!\n";
			return -1;
		}
		if (outfilename.empty()) {
			while (getline(fin, eq)) {
				std::cout << calc<float>(eq, details) << std::endl;
			}
		}
		else {
			ofstream fout;
			fout.open(outfilename);
			if (!fout.is_open()) {
				std::cerr << "Can't open the output file!\n";
				return -1;
			}
			while (getline(fin, eq)) {
				fout << calc<float>(eq, details) << std::endl;
			}
			fout.close();
		}
		fin.close();
	}

	else { // reading from command line
		std::cout << outfilename.empty();
		if (outfilename.empty()) {
			for (int i = 1; i < argc; i++) {
				if (argv[i] != "-v") {
					std::cout << calc<float>(argv[i], details) << std::endl;
				}
				if (argv[i] == "-o") {
					i++;
				}
			}
		}
		else {
			ofstream fout;
			fout.open(outfilename);
			if (!fout.is_open()) {
				std::cerr << "Can't open the output file!\n";
				return -1;
			}
			for (int i = 1; i < argc; i++) {
				if (argv[i] == "-o") {
					i++;
				}
				else if (argv[i] != "-v") {
					fout << calc<float>(argv[i], details) << std::endl;
				}
			}
			fout.close();
		}
	}
}
