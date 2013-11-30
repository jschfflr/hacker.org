#ifndef __SIMULATION_H__
#define __SIMULATION_H__

#include "common.h"

#include <thread>
#include "state.h"
#include "mutex.h"
#include "stack.h"
#include "heap.h"

class Simulation {
private:
	mutex stack_lock;
	heap<state*> heap;
	std::list<std::pair<int, int>> path;
	std::list<std::thread> threads;

	volatile bool running;
	unsigned long long samples;
	unsigned long long possibilities;
	static void thread(Simulation* pContext);

	void resolve(std::list<std::pair<int, int>>);

public:
	Simulation(Board& board);
	Simulation::~Simulation();
	std::list<std::pair<int,int>> run();
	std::list<Board> run(Board& b, std::list<std::pair<int,int>> path);
};


#endif