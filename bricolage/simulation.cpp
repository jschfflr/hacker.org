#include "simulation.h"
#include <iostream>

Simulation::Simulation(Board& board) {
	State start(board);
	queue.push(start);
}

std::list<std::pair<int,int>> Simulation::run() {
	while(!queue.empty()) {
		State state = (State)queue.front();
		queue.pop();

		auto areas = state.board.areas();

		// fetch all available areas from board
		// if len(areas) == 0 and not empty:
		//	invalid state -> don't process
		//  continue
		//
		if( areas.size() == 0 && !state.board.empty())
			continue; // 
			
		// for each area
		//		simulate click on area
		//		if empty:
		//			SOLUTION FOUND!!!
		//          return its path
		//		else:
		//			append to queue
		//
		for(auto it = areas.begin(); it != areas.end(); it++) {
			if( it->size() < 3)
				continue;

			State test = State(state);
			test.click(*it);
			if( test.board.empty() ) {
				// SOLUTION
				return test.clicks;
			} else {
				queue.push(test);
			}
		}
	}

	throw std::logic_error("Invalid State");
}