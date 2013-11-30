#ifndef __STATE_H__
#define __STATE_H__

#include "common.h"
#include "board.h"

class state {
public:
	Board board;
	std::list<std::pair<int,int>> clicks;

	state();
	state(Board board);
	void click(Area &a);
	void click(std::pair<int,int> click);

	std::string path() const ;

	inline bool operator <(const state& other) const {
		return board._areas.size() < other.board._areas.size();
	}

private:
	void update();
};

#endif