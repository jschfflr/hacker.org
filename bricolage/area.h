#ifndef __AREA_H__
#define __AREA_H__

#include <list>

struct Area {
	char color;
	std::list<std::pair<int,int>> points;

	Area(char color) {
		this->color = color;
	}

	int size() {
		return points.size();
	}
};

#endif