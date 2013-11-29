#ifndef __SIMULATION_H__
#define __SIMULATION_H__

#include <stack>
#include <mutex>
#include <thread>
#include <windows.h>
#include "state.h"
#include "mutex.h"

class Simulation {
private:
	mutex stack_lock;
	std::stack<state> stack;
	std::list<std::pair<int, int>> path;
	std::list<std::thread> threads;

	volatile bool running;
	static void thread(Simulation* pContext);

	void resolve(std::list<std::pair<int, int>>);

public:
	Simulation(Board& board);
	Simulation::~Simulation();
	std::list<std::pair<int,int>> run();
	std::list<Board> run(Board& b, std::list<std::pair<int,int>> path);
};


#endif