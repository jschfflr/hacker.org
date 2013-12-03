#ifndef __AREA_H__
#define __AREA_H__

#include <vector>

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

struct area {
	char _color;
	std::vector<point> points;

	area() {
		_color = -1;
	}

	area(char color) {
		_color = color;
	}

	int size() {
		return points.size();
	}

	inline bool operator <(const area& other) const {
		return points.size() < other.points.size();
	}

	inline char color() const { return _color; }
};

#endif