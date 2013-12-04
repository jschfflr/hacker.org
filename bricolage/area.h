#ifndef __AREA_H__
#define __AREA_H__

struct point {
	int x, y;

	point() {
		x = y = 0;
	}

	point(int x, int y) {
		this->x = x;
		this->y = y;
	}
};

class board;

struct area {
	friend class board;
	char _color;
	point _point;
	size_t _size;

	area() {
		_color = -1;
	}

	area(char color, point point = ::point(-1, -1), int size = 0) {
		_color = color;
		_point = point;
		_size = size;
	}

	~area() {

	}


	inline bool operator <(const area& other) const {
		return _size < other._size;
	}

	inline size_t size() const {return _size;}
	inline char color() const { return _color; }
	inline const point& point() const { return _point; }
};

#endif