#include "state.h"

State::State(Board board) {
	this->board = board;
}

void State::click(Area &area) {
	for( auto it = area.points.begin(); it != area.points.end(); it++)
		board.set(it->first, it->second, '.');

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

	int x = 0;
	while( board.get(x, 0) == '.' && x < board.width() - 1 )
		x++;
	
	while( board.get(x, 0) != '.' && x < board.width() - 1 )
		x++;
	
	while( x++ < board.width() - 1 ) {
		for(int y = 0; y < board.height(); y++) {
			board.set(x - 1, y, board.get(x, y) );
			board.set(x, y, '.');
		}
	}

	clicks.push_back( area.points.front() );
}