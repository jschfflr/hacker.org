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
	_areas = new heap<area*>(width * height / 2);
	if (board) {
		for (int x = 0; x < width; x++) {
			for (int y = 0; y < height; y++) {
				//convert from rows to cols for performance reason
				_data[y + x * height] = board[x + y * width];
			}
		}
		update();
	}
}

board::board(const board& board, bool calculate) {
	_width = board._width;
	_height = board._height;
	_data = new char[board._width * board._height];
	memcpy(_data, board._data, board._width * board._height);
	_areas = new heap<area*>(_width * _height / 2);
	
	if (calculate)
		update();
}

board::~board() {
	if( this->_data )
		delete [] this->_data;

	this->_data = 0;

	if (this->_areas) {
		for (size_t i = 0; i < _areas->size(); i++)
			delete this->_areas->get(i);

		delete _areas;
	}
		

	this->_areas = 0;
}

char board::get(int x, int y) const {
	if( !(0 <= x && x < _width) ||
		!(0 <= y && y < _height) )
		throw new std::range_error("board Index out of Range");

	return _data[y + x * _height];
}

char board::get(const point& p) const {
	if (!(0 <= p.x && p.x < _width) ||
		!(0 <= p.y && p.y < _height))
		throw new std::range_error("board Index out of Range");

	return _data[p.y + p.x * _height];
}

void board::set(int x, int y, char c) {
	if( !(0 <= x && x < _width) ||
		!(0 <= y && y < _height) )
		throw new std::range_error("board Index out of Range");
		
	_data[y + x * _height] = c;
}

void board::set(const point& p, char c) {
	if (!(0 <= p.x && p.x < _width) ||
		!(0 <= p.y && p.y < _height))
		throw new std::range_error("board Index out of Range");

	_data[p.y + p.x * _height] = c;
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

void board::floodfill(point point, area* area, char* data, void(*cb)(const ::point&, void*), void* param) {
	char* tmp;
	stack<::point> stack(_width * _height);
	char color = get( point );
	stack.push(point);

	if (!data) {
		tmp = new char[_width * _height];
		memcpy(tmp, _data, _width * _height);
	}
	else
		tmp = data;

	::point p;
	while (stack.pop(&p)) {
		if (!(0 <= p.x && p.x < _width) ||
			!(0 <= p.y && p.y < _height))
			continue; // Out of range

		if (tmp[p.y + p.x * _height] != color)
			continue;

		tmp[p.y + p.x * _height] = 0;
		area->_size++;
		if (cb)
			cb(p, param);

		stack.push(::point(p.x, p.y + 1));
		stack.push(::point(p.x, p.y - 1));
		stack.push(::point(p.x + 1, p.y));
		stack.push(::point(p.x - 1, p.y));
	}

	if (!data)
		delete[] tmp;
}

// Get all areas in the current field
void board::update() {
	::stack<point> stack(_width * _height);
	char* tmp = new char[_width * _height];
	memcpy(tmp, _data, _width * _height);

	char color;
	point c;
	for(int y = 0; y < _height; y++) {
		for(int x = 0; x < _width; x++) {

			color = get(x, y);
			if( color == 0 || color == '.' )
				continue;

			point p(x, y);
			area* area = new ::area(color, p);
			
			floodfill(p, area, tmp);

			if (area->size())
				_areas->insert(area);
			else
				delete area;
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

struct tmp {
	int left;
	int right;
	board* board;
};

static void clear(const point& p, void* param) {
	tmp* t = (tmp*)param;
	t->left = std::min(t->left, p.x);
	t->right = std::max(t->right, p.x);
	t->board->set(p, '-');
}

board* board::click(area& area) const {
	//initialise a new board with same dimensions
	board* result = new board(*this, false);

	// remove all fields that are part of the area
	// marking them with \0
	tmp params = {_width, 0, result};
	result->floodfill(area.point(), &area, 0, clear, &params);

	//loop through all cols
	for (int x = params.left; x < params.right; x++) {
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

	result->update();
	return result;
}