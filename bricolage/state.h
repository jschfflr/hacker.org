#ifndef __STATE_H__
#define __STATE_H__

#include "board.h"
#include "object.h"

class State : public Object {
public:
	Board board;
	std::list<std::pair<int,int>> clicks;

	State();
	State(Board board);
	void click(Area &a);
	void click(std::pair<int,int> click);

private:
	void update();
};

#endif