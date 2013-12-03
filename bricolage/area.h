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
	char color;
	std::vector<point> points;

	area(char color) {
		this->color = color;
	}

	int size() {
		return points.size();
	}
};

#endif