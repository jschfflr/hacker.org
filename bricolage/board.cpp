
#include "board.h"
#include <stack>

Board::Board() {
	_width = _height = 0;
	data = 0;
}

Board::Board(int width, int height, std::string board) {
	this->_width = width;
	this->_height = height;
	this->data = new char[width * height + 1];
	memcpy(this->data, board.data(), width * height);
	this->data[width * height] = '\0';
}

Board::Board(const Board& board) {
	*this = board;
}

Board& Board::operator =(const Board& board) {
	this->_width = board._width;
	this->_height = board._height;
	this->data = new char[board._width * board._height + 1];
	memcpy(this->data, board.data, board._width * board._height);
	this->data[board._width * board._height] = '\0';

	return *this;
}

Board::~Board() {
	if( this->data )
		delete [] this->data;
}

/*
char Board::get(int x, int y) const {
	if( !(0 <= x && x < _width) ||
		!(0 <= y && y < _height) )
		throw new std::range_error("Board Index out of Range");

	return data[x + y * _width];
}
*/

void Board::set(int x, int y, char c) {
	if( !(0 <= x && x < _width) ||
		!(0 <= y && y < _height) )
		throw new std::range_error("Board Index out of Range");
		
	data[x + y * _width] = c;
}

std::string Board::debug() {
	std::string output = "";
	for(int y = _height - 1; y >= 0; y--) {
		for(int x = 0; x < _width; x++) {
			output += ' ';
			output += get(x, y);
			output += ' ';
		}
		output += '\n';
	}

	return output;
}

Board::operator std::string() {
	return debug();
}

bool Board::empty() {
	for(int y = 0; y < _height; y++) {
		for(int x = 0; x < _width; x++) {
			if( get(x, y) != '.')
				return false;
		}
	}
	return true;
}

// Get all areas in the current field
std::list<Area> Board::areas() {
	std::stack<std::pair<int,int>> stack;
	std::list<Area> areas;
	std::string tmp = data;

	char color;
	for(int y = 0; y < _height; y++) {
		for(int x = 0; x < _width; x++) {

			color = tmp[x + y * _width];
			if( color == 0 || color == '.' )
				continue;

			Area area(color);
			stack.push(std::pair<int,int>(x, y));

			while(!stack.empty()) {
				auto c = stack.top();
				stack.pop();

				if( !(0 <= c.first && c.first < _width) ||
					!(0 <= c.second && c.second < _height) )
					continue; // Out of range

				if( tmp[c.first + c.second * _width] != color )
					continue;

				tmp[c.first + c.second * _width] = 0;
				area.points.push_back(c);
				
				stack.push(std::pair<int,int>(c.first, c.second+1));
				stack.push(std::pair<int,int>(c.first, c.second-1));
				stack.push(std::pair<int,int>(c.first+1, c.second));
				stack.push(std::pair<int,int>(c.first-1, c.second));
			}

			areas.push_back( area );
		}
	}

	return areas;
}

std::string Board::serialize() {
	std::string tmp = "";
	for(int y = 0; y < _height; y++) {
		for(int x = 0; x < _width; x++) {
			tmp += get(x, y);
		}
	}
	return tmp;
}

Board Board::Load(std::string src) {
	int _width = 0;
	int _height = 0;
	char* data = 0;

	throw std::runtime_error("Not Implemented Yet");
}