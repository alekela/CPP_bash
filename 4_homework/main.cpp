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


int check_cmd_line_flags(int argc, char* argv[], const char sym) {
	for (int i = 1; i < argc; i++) {
		if (argv[i][0] == '-' && argv[i][1] == sym) {
			return i;
		}
	}
	return 0;
}


int main(int argc, char* argv[]) {
	bool details;
	string eq;
	string outfilename = "";
	string infilename = "";

	vector<string> new_argv;
	for (int i = 1; i < argc; i++) {
		new_argv.push_back(argv[i]);
	}
	argc--;
	for (int i = 0; i < argc; i++) {
		if (new_argv[i] == "-h"){
			std::cout << "Some help info" << std::endl;
			return 0;
		}
		else if (new_argv[i] == "-v") {
			details = true;
			new_argv.erase(new_argv.begin() + i);
			i--;
			argc--;
		}
		else if (new_argv[i] == "-o"){
			if (i+1 < argc && isalpha(new_argv[i+1][0])) {
				outfilename = new_argv[i+1];
				new_argv.erase(new_argv.begin() + i);
				new_argv.erase(new_argv.begin() + i);
				i--;
				argc -= 2;
			}
			else {
				std::cerr << "No output file name!" << std::endl;
			}
		}
	}

	if (isalpha(new_argv[0][0])) {
		infilename = new_argv[0];
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
				std::cout << calc<float>(eq, details) << '\n' << std::endl;
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
				fout << calc<float>(eq, details) << '\n' << std::endl;
			}
			fout.close();
		}
		fin.close();
	}

	else { // reading from command line
		if (outfilename.empty()) {
			for (int i = 0; i < argc; i++) {
				std::cout << calc<float>(new_argv[i], details) << '\n' << std::endl;
			}
		}
		else {
			ofstream fout;
			fout.open(outfilename);
			if (!fout.is_open()) {
				std::cerr << "Can't open the output file!\n";
				return -1;
			}
			for (int i = 0; i < argc; i++) {
				fout << calc<float>(new_argv[i], details) << '\n' << std::endl;
			}
			fout.close();
		}
	}
}
