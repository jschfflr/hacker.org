#include "state.h"

#ifdef _DEBUG   
#ifndef DBG_NEW      
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )      
#define new DBG_NEW   
#endif
#endif  // _DEBUG


State::State() {
	this->board = Board();
}

State::State(Board board) {
	this->board = board;
}

void State::click(std::pair<int,int> click) {
	auto areas = board.areas();
	for( auto area: areas) {
		auto it = std::find(area.points.begin(), area.points.end(), click);
		if( it != area.points.end() )
			return State::click( area );
	}
}

void State::click(Area &area) {
	for( auto it = area.points.begin(); it != area.points.end(); it++)
		board.set(it->first, it->second, '.');

	update();

	clicks.push_back( area.points.front() );
}

void State::update() {
	int changes = 0;
	do {
		changes = 0;
		for(int x = 0; x < board.width(); x++) {
			for(int y = 1; y < board.height(); y++) {
				if( board.get(x, y) != '.' && board.get(x, y - 1) == '.' ) {
					board.set(x, y - 1, board.get(x, y));
					board.set(x, y, '.');
					changes += 1;
				}
			}
		}
	} while( changes );

	do {
		int x = 0;
		changes = 0;
		while( board.get(x, 0) == '.' && x < board.width() - 1 )
			x++;
	
		while( board.get(x, 0) != '.' && x < board.width() - 1 )
			x++;

		int tmp = x;
		while( board.get(tmp, 0) == '.' && tmp < board.width() - 1)
			tmp++;

		if( tmp == board.width() - 1 )
			break;
	
		while( x++ < board.width() - 1 ) {
			changes++;
			for(int y = 0; y < board.height(); y++) {
				board.set(x - 1, y, board.get(x, y) );
				board.set(x, y, '.');
			}
		}
	} while( changes );

}