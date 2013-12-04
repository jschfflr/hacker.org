#ifndef __SIMULATION_H__
#define __SIMULATION_H__

#include "common.h"

#include <list>
#include <thread>
#include "state.h"
#include "mutex.h"
#include "heap.h"

class simulation {
private:
	mutex lock;
	state* initial;
	std::list<std::thread> threads;

	std::string path;
	size_t samples;
	size_t possibilities;
	const state* result;
	volatile bool running;
	//void thread();
	void resolve(const state* state);
	//static void thread_wrapper(simulation* pContext);
	void depth_first_search(state* initial);
	static void dfs_wrapper(simulation* self, state* initial);
	

public:
	simulation(const board& board);
	~simulation();
	//void run(std::string* path);
	void dfs(std::string* path);
};


#endif