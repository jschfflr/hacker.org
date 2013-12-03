#include "state.h"

state::state() {
	_parent = 0;
	_board = 0;
	_click = point(-1, -1);
}

state::state(const ::board* board) {
	_parent = 0;
	_board = board;
	_click = point(-1, -1);
}

state::state(state* parent, const ::board* board, area& click) {
	_click = click.points[0];
	_parent = parent;
	_board = board->click(click);
}

