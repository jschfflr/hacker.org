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
	heap<area*>* _areas;

	void floodfill(point point, area* area, char* data = 0, void(*cb)(const ::point&, void* buffer) = 0, void* param = 0);
public:
	board();
	~board();
	board(const board& board, bool calculate = true);
	board(int width, int height, const char* board);

	char get(int x, int y) const;
	char get(const point& p) const;
	void set(int x, int y, char c);
	void set(const point& p, char c);

	std::string debug() const;
	operator std::string();
	bool empty() const;

	// Get all areas in the current field
	void update();
	std::string serialize();

	inline int width() const { return _width; }
	inline int height() const { return _height; }
	inline heap<area*>* areas() const { return _areas; }

	board* click(area& click) const;
};


#endif