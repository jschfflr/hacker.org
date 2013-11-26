#ifndef __STATE_H__
#define __STATE_H__

#include "board.h"

class State {
public:
	Board board;
	std::list<std::pair<int,int>> clicks;

	State(Board board);
	void click(Area &a);
};

#endif