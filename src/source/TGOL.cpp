// This file is distributed under the MIT License. For full text of the license see file named "LICENSE" in project's root directory.
// Copyright (c) 2014 nabijaczleweli
//
// Designed for gcc 4.8.1-dw2 - works flawlessly there. For crashes / compilation faults on other compilers please contact the author.
// Contributors: nabijaczleweli (nabijaczleweli@gmail.com)

#include <iostream>
#include <random>
#include <ctime>

#include <armadillo>

using namespace std;
using namespace arma;

template<class T>
void tick(Mat<T> &, const unsigned int &, const unsigned int &);
template<class T>
void pre_tick(Mat<T> &, unsigned int &, unsigned int &);
template<class T>
unsigned int get_alive_neighbours(const Mat<T> &, const unsigned int &, const unsigned int &);
pair<unsigned int, unsigned int> get_level_size(const string &);
bool is_level_an_actual_level(const string &);
void print_bad_information(const string & filename, unsigned int line, unsigned int character, const char * const message);

int main(int, char * argv[]) {
	cout.sync_with_stdio(false);
	cin.sync_with_stdio(false);

	string filename;
	for(unsigned int idx = 1; argv[idx]; ++idx)
		filename = argv[idx];
	if(filename.size())
		if(!is_level_an_actual_level(filename)) {
			cout << '\"' << filename << "\" is not an actual level! Switching to auto-generate!\n\n";
			filename.erase();
			filename.shrink_to_fit();
		}

	unsigned int life_x = 60,
	             life_y = 30;
	if(filename.size()) {
		auto size = get_level_size(filename);
		life_x = size.first;
		life_y = size.second;
		cout << life_x << ',' << life_y;
	}
	Mat<char> the_map(life_y, life_x, fill::zeros);
	srand(time(NULL));

	if(filename.size()) {

	} else
		for(unsigned int y = 0; y < life_y; ++y)
			for(unsigned int x = 0; x < life_x; ++x)
				the_map(y, x) = !(rand() % 9);
	/*the_map(7, 2) = false; // Lightweight spaceship
	the_map(7, 3) = true;
	the_map(7, 4) = true;
	the_map(7, 5) = true;
	the_map(7, 6) = true;

	the_map(8, 2) = true;
	the_map(8, 3) = false;
	the_map(8, 4) = false;
	the_map(8, 5) = false;
	the_map(8, 6) = true;

	the_map(9, 2) = false;
	the_map(9, 3) = false;
	the_map(9, 4) = false;
	the_map(9, 5) = false;
	the_map(9, 6) = true;

	the_map(10, 2) = true;
	the_map(10, 3) = false;
	the_map(10, 4) = false;
	the_map(10, 5) = true;
	the_map(10, 6) = false;*//* // 1-line replicator
	the_map(10, 4) = true;
	the_map(10, 5) = true;
	the_map(10, 6) = true;
	the_map(10, 7) = true;
	the_map(10, 8) = true;
	the_map(10, 9) = true;
	the_map(10, 10) = true;
	the_map(10, 11) = true;

	the_map(10, 12) = false;

	the_map(10, 13) = true;
	the_map(10, 14) = true;
	the_map(10, 15) = true;
	the_map(10, 16) = true;
	the_map(10, 17) = true;

	the_map(10, 18) = false;
	the_map(10, 19) = false;
	the_map(10, 20) = false;

	the_map(10, 21) = true;
	the_map(10, 22) = true;
	the_map(10, 23) = true;

	the_map(10, 24) = false;
	the_map(10, 25) = false;
	the_map(10, 26) = false;
	the_map(10, 27) = false;
	the_map(10, 28) = false;
	the_map(10, 29) = false;

	the_map(10, 30) = true;
	the_map(10, 31) = true;
	the_map(10, 32) = true;
	the_map(10, 33) = true;
	the_map(10, 34) = true;
	the_map(10, 35) = true;
	the_map(10, 36) = true;

	the_map(10, 37) = false;

	the_map(10, 38) = true;
	the_map(10, 39) = true;
	the_map(10, 40) = true;
	the_map(10, 41) = true;
	the_map(10, 42) = true;*/
	for(unsigned int y = 0; y < life_y; ++y) {
		for(unsigned int x = 0; x < life_x; ++x)
			cout << (the_map(y, x) ? '\262' : '\260');
		cout << '\n';
	}
	string line;
	while(true) {
		getline(cin, line);
		if(!line.find("q"))
			break;
		else if(!line.find("x+")) {
			if(line.size() == 2)
				line += '1';
			the_map.resize(life_y, life_x += atoi(line.substr(2).c_str()));
			cout << "\nNew dimensions: x = " << life_x << ", y = " << life_y << ".\n";
			continue;
		} else if(!line.find("y+")) {
			if(line.size() == 2)
				line += '1';
			the_map.resize(life_y += atoi(line.substr(2).c_str()), life_x);
			cout << "\nNew dimensions: x = " << life_x << ", y = " << life_y << ".\n";
			continue;
		} else if(!line.find("t")) {
			if(line.size() == 1)
				line += "10";
			unsigned int got = atoi(line.substr(1).c_str());
			for(unsigned int i = 1; i < got; ++i) {
				pre_tick(the_map, life_x, life_y);
				tick(the_map, life_x, life_y);
			}
		} else if(!line.find("fs")) {
			line.erase(remove_if(line.begin(), line.end(), [](const char & x){return isspace(x);}), line.end());
			if(line.size() == 2)
				line += "10,10";
			else if(!line.find(","))
				line += ",10";
			else if(!isdigit(line[2])) {
				line = line.substr(3);
				line = "fs10" + line;
			} else if(!isdigit(line[line.find(",") + 1]))
				line = line.substr(0, line.find(",") + 1) + "10";

			unsigned int y = atoi(line.substr(line.find(",") + 1).c_str()),
			             x = atoi(line.substr(2).c_str());
			if(life_y <= y)
				the_map.resize(life_y = (y + 1), life_x);
			if(life_x <= x)
				the_map.resize(life_y, life_x = (x + 1));
			the_map(y, x) = true;
			continue;
		} else if(!line.find("fr")) {
			line.erase(remove_if(line.begin(), line.end(), [](const char & x){return isspace(x);}), line.end());
			if(line.size() == 2)
				line += "10,10";
			else if(!line.find(","))
				line += ",10";
			else if(!isdigit(line[2])) {
				line = line.substr(3);
				line = "fr10" + line;
			} else if(!isdigit(line[line.find(",") + 1]))
				line = line.substr(0, line.find(",") + 1) + "10";

			unsigned int y = atoi(line.substr(line.find(",") + 1).c_str()),
			             x = atoi(line.substr(2).c_str());
			if(life_y < y)
				the_map.resize(life_y = y, life_x);
			if(life_x < x)
				the_map.resize(life_y, life_x = x);
			the_map(y, x) = false;
			continue;
		} else if(!line.find("p")) {
			for(unsigned int y = 0; y < life_y; ++y) {
				for(unsigned int x = 0; x < life_x; ++x)
					cout << (the_map(y, x) ? '\262' : '\260');
				cout << '\n';
			}
			continue;
		} else if(!line.find("?")) {
			cout << "Conway's Game of Life:\n\t"
			           "Help:\n\t\t" // TODO : Maybe some kinda table with commands or somethin'?
								   "\"q\" Does the cleanup an exits.\n\t\t"
									 "\"x+[NUMBER]\" Extends number of columns of the gamefield by NUMBER. NUMBER defaults to 10.\n\t\t"
									 "\"y+[NUMBER]\" Extends number of rows of the gamefield by NUMBER. NUMBER defaults to 10.\n\t\t"
									 "\"t[NUMBER]\" Ticks gamefield NUMBER times, then prints it. NUMBER defaults to 10.\n\t\t"
									 "\"fs[[NUMBER1][, NUMBER2]]\" Sets cell at {x = NUMBER1, y = NUMBER2} to alive. Both NUMBER1 and NUMBER2 default to 10.\n\t\t"
									 "\"fr[[NUMBER1][, NUMBER2]]\" Sets cell at {x = NUMBER1, y = NUMBER2} to dead. Both NUMBER1 and NUMBER2 default to 10.\n\t\t"
									 "\"p\" Shows gamefield without ticking.\n\t\t"
									 "\"<any other expression>\" Ticks the gamefield, then prints it.\n\t"
								"Info:\n\t\t"
								  "Follows standard Conway's rules of the Conway's game of life.\n\t\t"
									"Field is randomly populated at the beginning.\n\t\t"
									"Field automatically resizes itself as needed.\n\t"
								"Authors:\n\t\t"
								  "Nabijaczleweli: original authorism.\n\t\t"
									"More to come?\n\t"
								"Report bugs to <nabijaczlewei@gmail.com>\n";
			continue;
		}
		pre_tick(the_map, life_x, life_y);
		tick(the_map, life_x, life_y);
		for(unsigned int i = 0; i < (life_y * .75); ++i)
			cout << '\n';
		for(unsigned int y = 0; y < life_y; ++y) {
			for(unsigned int x = 0; x < life_x; ++x)
				cout << (the_map(y, x) ? '\262' : '\260');
			cout << '\n';
		}
	}
}

template<class T>
void tick(Mat<T> & map, const unsigned int & x_size, const unsigned int & y_size) {
	typename remove_reference<decltype(map)>::type new_map(map);

	for(unsigned int y = 1; y < (y_size - 1); ++y)
		for(unsigned int x = 1; x < (x_size - 1); ++x)
			switch(get_alive_neighbours(map, y, x)) {
				case 0 :
				case 1 :
				case 4 :
				case 5 :
				case 6 :
				case 7 :
				case 8 : new_map(y, x) = false; break;
				case 2 : break;
				case 3 : new_map(y, x) = true; break;
			}

	map = new_map;
}
template<class T>
unsigned int get_alive_neighbours(const Mat<T> & map, const unsigned int & posY, const unsigned int & posX) {
	return map(posY - 1, posX - 1) +
	       map(posY - 1, posX    ) +
	       map(posY - 1, posX + 1) +
	       map(posY,     posX - 1) +
	       map(posY,     posX + 1) +
	       map(posY + 1, posX - 1) +
	       map(posY + 1, posX    ) +
	       map(posY + 1, posX + 1);
}
template<class T>
void pre_tick(Mat<T> & map, unsigned int & x_size, unsigned int & y_size) {
	for(unsigned int y = 0; y < y_size; ++y)
		if(map(y, x_size - 2)) {
			map.resize(y_size, ++x_size);
			return;
		} else if(map(y, x_size - 1)) {
			map.resize(y_size, x_size += 2);
			return;
		}
	for(unsigned int x = 0; x < x_size; ++x)
		if(map(y_size - 2, x)) {
			map.resize(++y_size, x_size);
			return;
		} else if(map(y_size - 1, x)) {
			map.resize(y_size += 2, x_size);
			return;
		}

	for(unsigned int y = 0; y < y_size; ++y)
		if(map(y, 1)) {
			typename remove_reference<decltype(map)>::type new_map(y_size, ++x_size, fill::zeros);
			for(unsigned int sx = 1; sx < x_size; ++sx)
				new_map.col(sx) = map.col(sx - 1);
			map.~Mat<typename remove_reference<decltype(map(0, 0))>::type>();
			map = new_map;
			return;
		} else if(map(y, 0)) {
			typename remove_reference<decltype(map)>::type new_map(y_size, x_size += 2, fill::zeros);
			for(unsigned int sx = 2; sx < x_size; ++sx)
				new_map.col(sx) = map.col(sx - 2);
			map.~Mat<typename remove_reference<decltype(map(0, 0))>::type>();
			map = new_map;
			return;
		}
	for(unsigned int x = 0; x < x_size; ++x)
		if(map(1, x)) {
			typename remove_reference<decltype(map)>::type new_map(++y_size, x_size, fill::zeros);
			for(unsigned int sy = 1; sy < y_size; ++sy)
				new_map.row(sy) = map.row(sy - 1);
			map.~Mat<typename remove_reference<decltype(map(0, 0))>::type>();
			map = new_map;
			return;
		} else if(map(0, x)) {
			typename remove_reference<decltype(map)>::type new_map(y_size += 2, x_size, fill::zeros);
			for(unsigned int sy = 2; sy < y_size; ++sy)
				new_map.row(sy) = map.row(sy - 2);
			map.~Mat<typename remove_reference<decltype(map(0, 0))>::type>();
			map = new_map;
			return;
		}
}

pair<unsigned int, unsigned int> get_level_size(const string & filename) {
	ifstream input_file(filename);

	input_file.ignore(6);
	string temporary_input;
	getline(input_file, temporary_input);
	unsigned int x_size = atoi(temporary_input.c_str());

	input_file.ignore(7);
	getline(input_file, temporary_input);
	unsigned int y_size = atoi(temporary_input.c_str());

	return make_pair(x_size, y_size);
}

void print_bad_information(const string & filename, unsigned int line, unsigned int character, const char * const message) {
	cerr << filename << ':' << line << ':' << character << ": " << message << '\n';
	ifstream input_file(filename);
	for(unsigned int curline = 0; curline < line; ++curline)
		input_file.ignore(numeric_limits<streamsize>::max(), '\n');
	string output_line;
	getline(input_file, output_line);
	cerr << output_line << '\n';
	for(unsigned int i = 0; i < character; ++i)
		cerr << ' ';
	cerr << '^' << endl;
}

bool is_level_an_actual_level(const string & filename) {
	#define TESTFILEFORCHARACTER(character_to_test_against)\
	 if((chr = input_file.get()) != character_to_test_against) {\
		 char * buffer = new char[35 + ((chr == '\n') * 4)];\
		 memcpy(buffer, "error: \'", 8);\
		 buffer[8] = character_to_test_against;\
		 memcpy(buffer + 9, "\' expected, but \'", 17);\
		 if(chr != '\n') {\
		 	buffer[26] = chr;\
		 	memcpy(buffer + 27, "\' found", 8);\
		 } else\
		 	memcpy(buffer + 25, "newline found", 14);\
		 print_bad_information(filename, curline, curchar, buffer);\
		 delete[] buffer;\
		 ++curchar;\
		 not_ok = true;\
	 }

	ifstream input_file(filename);
	unsigned int curline = 0, curchar = 0;
	bool not_ok = false;
	int chr;

	for(unsigned int i = 0; i < 6; ++i)
		TESTFILEFORCHARACTER("width="[i])
	while(input_file.peek() != '\n' && input_file.peek() != ifstream::traits_type::eof()) {
		if(!isdigit(chr = input_file.get())) {
			char * buffer = new char[49];
			memcpy(buffer, "error: decimal character expected, but \'", 40);
			buffer[40] = chr;
			memcpy(buffer + 41, "\' found", 9);
			print_bad_information(filename, curline, curchar, buffer);
			delete[] buffer;
			not_ok = true;
		}
		++curchar;
	}
	TESTFILEFORCHARACTER('\n')
	++curline;
	curchar = 0;

	for(unsigned int i = 0; i < 7; ++i)
		TESTFILEFORCHARACTER("height="[i])
	while(input_file.peek() != '\n' && input_file.peek() != ifstream::traits_type::eof()) {
		if(!isdigit(chr = input_file.get())) {
			char * buffer = new char[49];
			memcpy(buffer, "error: decimal character expected, but \'", 40);
			buffer[40] = chr;
			memcpy(buffer + 41, "\' found", 9);
			print_bad_information(filename, curline, curchar, buffer);
			delete[] buffer;
			not_ok = true;
		}
		++curchar;
	}
	TESTFILEFORCHARACTER('\n')
	++curline;
	curchar = 0;

	while(input_file.peek() != ifstream::traits_type::eof()) {
		chr = input_file.get();
		if(!(chr == '0' || chr == '1' || chr == '\n')) {
			char * buffer = new char[59];
			memcpy(buffer, "error: binary character or newline expected, but \'", 50);
			buffer[50] = chr;
			memcpy(buffer + 51, "\' found", 9);
			print_bad_information(filename, curline, curchar, buffer);
			delete[] buffer;
			not_ok = true;
		} else if(chr == '\n') {
			++curline;
			curchar = 0;
		} else
			++curchar;
	}

	return !not_ok;

	#undef TESTFILEFORCHARACTER
}