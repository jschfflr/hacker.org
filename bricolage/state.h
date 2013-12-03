#ifndef __STATE_H__
#define __STATE_H__

#include "common.h"
#include "board.h"
#include "area.h"

class state {
public:
	state* _parent;
	board* _board;
	point _click;

	state();
	state(state* parent, board* board, area& click);
	void click(const area& a);
	void click(point click);

	inline bool operator <(const state& other) const {
		return _board->areas()->size() < other._board->areas()->size();
	}

private:
	void update();
};

#endif