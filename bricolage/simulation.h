#ifndef __SIMULATION_H__
#define __SIMULATION_H__

#include <queue>
#include "state.h"

class Simulation {
private:
	std::queue<State> queue;

public:
	Simulation(Board& board);
	std::list<std::pair<int,int>> run();
};


#endif