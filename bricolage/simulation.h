#ifndef __SIMULATION_H__
#define __SIMULATION_H__

#include <stack>
#include "state.h"

class Simulation {
private:
	std::stack<State> queue;

public:
	Simulation(Board& board);
	std::list<std::pair<int,int>> run();
	std::list<Board> run(Board& b, std::list<std::pair<int,int>> path);
};


#endif