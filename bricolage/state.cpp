#include "state.h"

state::state() {
	_parent = 0;
	_board = 0;
	_click = point(-1, -1);
}

state::state(const ::board& board) {
	_parent = 0;
	_board = new ::board(board, true);
	_click = point(-1, -1);
}

state::state(const state& other) {
	throw std::runtime_error("Copy State");
}

state::~state() {
	if (_board)
		delete _board;
}

state::state(state* parent, area& click) {
	_click = click.point();
	_parent = parent;
	_board = parent->board()->click(click);
}

