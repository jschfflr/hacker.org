#ifndef __BOARD_H__
#define __BOARD_H__

#include <string>

#include "area.h"

class Board {
public:
	enum {
		EMPTY = '.',
	};

private:
	int _width;
	int _height;
	std::string data;

public:
	Board();

	Board(int width, int height, std::string board);
	char get(int x, int y) const ;
	void set(int x, int y, char c);

	std::string debug();
	operator std::string();
	bool empty();

	// Get all areas in the current field
	std::list<Area> areas();
	std::string serialize();

	inline int width() const { return _width; }
	inline int height() const { return _height; }

	static Board Load(std::string src);
};


#endif