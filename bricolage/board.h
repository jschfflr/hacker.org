#ifndef __BOARD_H__
#define __BOARD_H__

#include "common.h"

#include <string>
#include "area.h"
#include "heap.h"

class board{
private:
	int _width;
	int _height;
	char* _data;
	heap<area>* _areas;

public:
	board();
	~board();
	board(const board& board, bool calculate = true);
	board(int width, int height, const char* board);
	board& operator=(const board& );

	char get(int x, int y) const;
	void set(int x, int y, char c);

	std::string debug() const;
	operator std::string();
	bool empty() const;

	// Get all areas in the current field
	void areas(heap<area>& areas);
	std::string serialize();

	inline int width() const { return _width; }
	inline int height() const { return _height; }
	inline heap<area>* areas() const { return _areas; }

	board* click(area& click) const;
};


#endif