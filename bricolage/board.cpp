#include <algorithm>

#include "board.h"
#include "stack.h"


board::board() {
	_width = _height = 0;
	_data = 0;
	_areas = 0;
}

board::board(int width, int height, const char* board) {
	_width = width;
	_height = height;
	_data = new char[width * height];
	_areas = new heap<area>(width * height);
	if (board) {
		for (int x = 0; x < width; x++) {
			for (int y = 0; y < height; y++) {
				//convert from rows to cols for performance reason
				_data[y + x * height] = board[x + y * width];
			}
		}
		areas(*_areas);
	}
}

board::board(const board& board) {
	_width = board._width;
	_height = board._height;
	_data = new char[board._width * board._height];
	memcpy(_data, board._data, board._width * board._height);
	if (board._areas)
		_areas = new heap<area>(board._areas->size());
	else {
		_areas = new heap<area>(_width * _height);
		areas(*_areas);
	}
}

board& board::operator =(const board& board) {
	this->_width = board._width;
	this->_height = board._height;
	if (this->_data) delete[] this->_data;
	this->_data = new char[board._width * board._height];
	memcpy(this->_data, board._data, board._width * board._height);
	
	if (this->_areas) delete[] this->_areas;
	if (board._areas)
		this->_areas = new heap<area>(board._areas->size());
	else {
		_areas = new heap<area>(_width * _height);
		areas(*_areas);
	}
	
	return *this;
}

board::~board() {
	if( this->_data )
		delete [] this->_data;

	this->_data = 0;

	if (this->_areas)
		delete[] this->_areas;

	this->_areas = 0;
}

char board::get(int x, int y) const {
	if( !(0 <= x && x < _width) ||
		!(0 <= y && y < _height) )
		throw new std::range_error("board Index out of Range");

	return _data[y + x * _height];
}

void board::set(int x, int y, char c) {
	if( !(0 <= x && x < _width) ||
		!(0 <= y && y < _height) )
		throw new std::range_error("board Index out of Range");
		
	_data[y + x * _height] = c;
}

std::string board::debug() const {
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

board::operator std::string() {
	return debug();
}

bool board::empty() const {
	for(int y = 0; y < _height; y++) {
		for(int x = 0; x < _width; x++) {
			if( get(x, y) != '.')
				return false;
		}
	}
	return true;
}

// Get all areas in the current field
void board::areas(heap<area>& areas) {
	::stack<point> stack(_width * _height);
	char* tmp = new char[_width * _height];
	memcpy(tmp, _data, _width * _height);

	char color;
	for(int y = 0; y < _height; y++) {
		for(int x = 0; x < _width; x++) {

			color = get(x, y);
			if( color == 0 || color == '.' )
				continue;

			area area(color);
			stack.push(point(x, y));

			while(stack.size()) {
				auto c = stack.top();
				stack.pop();

				if( !(0 <= c.x && c.x < _width) ||
					!(0 <= c.y && c.y < _height) )
					continue; // Out of range

				if( tmp[c.y + c.x * _height] != color )
					continue;

				tmp[c.y + c.x * _height] = 0;
				area.points.push_back(c);
				
				stack.push(point(c.x, c.y  +1));
				stack.push(point(c.x, c.y - 1));
				stack.push(point(c.x + 1, c.y));
				stack.push(point(c.x - 1, c.y));
			}

			if (area.size())
				areas.insert( area );
		}
	}
	delete[] tmp;
}

std::string board::serialize() {
	std::string tmp = "";
	for(int y = 0; y < _height; y++) {
		for(int x = 0; x < _width; x++) {
			tmp += get(x, y);
		}
	}
	return tmp;
}

board* board::click(area& area) const {
	//initialise a new board with same dimensions
	board* result = new board(*this);

	int left = _width, right = 0;
	// remove all fields that are part of the area
	// marking them with \0
	for (auto it = area.points.begin(); it != area.points.end(); it++) {
		left = std::min(left, it->x);
		right = std::max(right, it->x);
		result->set(it->x, it->y, '-');
	}
	

	//loop through all cols
	for (int x = left; x < _width; x++) {
		// if we find a cleared cell
		char *src, *dst;
		for (src = dst = &result->_data[x * _height]; src < &result->_data[(x + 1) * _height]; src++) {
			*dst = *src;
			if (*dst != '-' && *dst != '.' )
				dst++;
		}
		
		//memset(dst, '.', _height - int( dst - result->_data[x * _height]));
		while (dst < &result->_data[(x + 1) * _height])
			*dst++ = '.';
	}

	char *src = result->_data, *dst;
	while ((*src == '-' || *src == '.') && src < &result->_data[_width * _height])
		src += _height;

	for (dst = src; src < &result->_data[_width * _height]; src += _height) {
		if (src != dst)
			memcpy(dst, src, _height);
		if (*dst != '-' && *dst != '.')
			dst+= _height;
	}

	while (dst < &result->_data[_width * _height])
		*dst++ = '.';

	return result;
}