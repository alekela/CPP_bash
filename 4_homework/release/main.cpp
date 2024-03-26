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
int calc(string eq, bool details, ostream* out) {
	if (not check_brackets(eq)) {
		(*out) << "Incorrect equation!\n" << std::endl;
		return -1;
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

	int index;

	for (int i = 0; i < signs.size(); i++) {
		index = get<1>(signs[i]);

		if (details) { // check if user wants more detailed info (flag -v)
			(*out) << numbers[index] << ' ' << get<2>(signs[i]) << ' ' << numbers[index + 1] << " = ";
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
			(*out) << numbers[index] << std::endl;
		}

		for (int j = i + 1; j < signs.size(); j++) { // deleting from numbers so changing indexes in signs
			if (get<1>(signs[j]) > index) {
				get<1>(signs[j])--;
			}
		}
		numbers.erase(numbers.begin() + index + 1); // deleting number that we used
	}

	(*out) << "Answer: " << numbers[0] << '\n' << std::endl;
	return 0;
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
				std::cerr << "No output file name!\n" << std::endl;
			}
		}
	}
	if (isalpha(new_argv[0][0])) {
		infilename = new_argv[0];
	}

	// opening read/write files
	ifstream fin;
	ofstream fout;
	if (!infilename.empty()) {
		fin.open(infilename);
		if (!fin.is_open()) {
			std::cerr << "Can't open the input file!\n" << std::endl;
			return -1;
		}
	}
	if (!outfilename.empty()) {
		fout.open(outfilename);
		if (!fout.is_open()) {
			std::cerr << "Can't open the output file!\n" << std::endl;
			return -1;
		}
	}

	if (!infilename.empty()) { // reading from file
		if (outfilename.empty()) {
			while (getline(fin, eq)) {
				calc<float>(eq, details, &cout);
			}
		}
		else {
			while (getline(fin, eq)) {
				calc<float>(eq, details, &fout);
			}
		}
	}

	else { // reading from command line
		if (outfilename.empty()) {
			for (int i = 0; i < argc; i++) {
				calc<float>(new_argv[i], details, &cout);
			}
		}
		else {
			for (int i = 0; i < argc; i++) {
				calc<float>(new_argv[i], details, &fout);
			}
		}
	}

	if (fin.is_open()) {
		fin.close();
	}
	if (fout.is_open()) {
		fout.close();
	}
}
