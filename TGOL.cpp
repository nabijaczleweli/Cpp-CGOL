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

int main() {
	cout.sync_with_stdio(false);
	cin.sync_with_stdio(false);
	bool (*isnumeric)(const char &) = [](const char & c){return (c >= '0' && c <= '9');};
	unsigned int life_x = 60,
	             life_y = 30;
	Mat<char> the_map(life_y, life_x, fill::zeros);
	srand(time(NULL));

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
			if(!isnumeric(line[1]))
				line += "10";
			unsigned int got = atoi(line.substr(1).c_str());
			for(unsigned int i = 0; i < got; ++i) {
				pre_tick(the_map, life_x, life_y);
				tick(the_map, life_x, life_y);
			}
		} else if(!line.find("fs")) {
			line.erase(remove_if(line.begin(), line.end(), [](const char & x){return isspace(x);}), line.end());
			if(line.size() == 2)
				line += "10,10";
			else if(!line.find(","))
				line += ",10";
			else if(!isnumeric(line[2])) {
				line = line.substr(3);
				line = "fs10" + line;
			} else if(!isnumeric(line[line.find(",") + 1]))
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
			else if(!isnumeric(line[2])) {
				line = line.substr(3);
				line = "fr10" + line;
			} else if(!isnumeric(line[line.find(",") + 1]))
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
	the_map.~Mat<typename remove_reference<decltype(the_map(0, 0))>::type>();
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