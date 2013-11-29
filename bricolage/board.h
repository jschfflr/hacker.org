#ifndef __BOARD_H__
#define __BOARD_H__

#include <string>

#include "area.h"

class Board{
public:
	enum {
		EMPTY = '.',
	};

private:
	int _width;
	int _height;
	char* data;

public:
	std::list<Area> _areas;

	Board();
	~Board();
	Board::Board(const Board& board);
	Board(int width, int height, std::string board);
	Board& operator=(const Board& );

	inline char get(int x, int y) const {
		if( !(0 <= x && x < _width) ||
			!(0 <= y && y < _height) )
			throw new std::range_error("Board Index out of Range");

		return data[x + y * _width];	
	};
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