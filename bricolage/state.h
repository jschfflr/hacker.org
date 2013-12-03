#ifndef __STATE_H__
#define __STATE_H__

#include "common.h"
#include "board.h"
#include "area.h"

class state {
	state* _parent;
	const board* _board;
	point _click;

public:
	state();
	state(const board* board);
	state(state* parent, const board* board, area& click);
	void click(const area& a);
	void click(point click);

	inline bool operator <(const state& other) const {
		return _board->areas()->size() < other._board->areas()->size();
	}

	inline const state* parent() const { return _parent; }
	inline const board* board() const { return _board; }
	inline const point click() const { return _click;  }
};

#endif