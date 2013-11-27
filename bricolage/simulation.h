#ifndef __SIMULATION_H__
#define __SIMULATION_H__

#include <stack>
#include <mutex>
#include <windows.h>
#include "state.h"

class Simulation {
private:
	std::stack<State> stack;
	std::mutex stack_lock;
	volatile bool running;
	static DWORD WINAPI thread(void* pContext);

public:
	Simulation(Board& board);
	Simulation::~Simulation();
	std::list<std::pair<int,int>> run();
	std::list<Board> run(Board& b, std::list<std::pair<int,int>> path);
};


#endif