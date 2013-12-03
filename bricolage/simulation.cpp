#include "simulation.h"
#include <iostream>
#include <windows.h>
#include <process.h>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include "stats.h"
#include "monitor.h"


#include <stack>

simulation::simulation(const board& board):
	heap(4096){
	samples = 0;
	possibilities = 0;

	heap.insert(new state(&board) );
}

simulation::~simulation() {
	//StatsManager::Get().RemoveVar("stack");
	for (size_t i = 0; i < heap.size(); i++) {
		delete heap[i];
	}
}

void simulation::resolve(const state* state) {
	heap_lock.lock();
	if (running)
		this->result = state;

	running = false;
	heap_lock.release();
}

void simulation::thread_wrapper(simulation* self) {
	//try {
		self->thread();
	//}
	//catch (std::exception e) {
	//	monitor::_emit(monitor::event("exception") << "thread" << e.what());
	//	terminate();
	//}
}

void simulation::thread() {
	state* state;
	while (running) {
		heap_lock.lock();

		if (heap.empty()) {
			// Nothing to do right now
			heap_lock.unlock();
			Sleep(1);
			continue;
		}
		else {
			heap.pop(state);
			samples++;
			possibilities += state->board()->areas()->size();
			heap_lock.unlock();
		}

		std::cout << state->board()->debug() << std::endl;

		if (state->board()->areas()->size() == 0 && !state->board()->empty()) {
			delete state; //No more moves here
			continue; // 
		}

		area area;
		while (state->board()->areas()->pop(area)) {
			// Can not click this area
			if (area.size() < 3)
				continue;

			// perform click
			::state *p = new ::state(state, state->board(), area);
			std::cout << area.points[0].x << " " << area.points[0].y << std::endl;
			
			if (p->board()->empty()) {
				return resolve(p);
			}
			else {
				heap_lock.lock();
				heap.insert(p);
				heap_lock.unlock();
			}
		}

		delete state;
	}
}

void simulation::run(std::string& path) {
	running = true;
	timer t;
	for (unsigned int i = 0; i < std::thread::hardware_concurrency(); i++)
		threads.push_back(std::thread(thread_wrapper, this));

	for (auto it = threads.begin(); it != threads.end(); it++)
		it->join();

	monitor::_emit(monitor::event("samples") << samples << t.get());
	monitor::_emit(monitor::event("possibilities") << possibilities << t.get());

	std::stringstream result;
	const state* p = this->result;
	result << std::hex << std::setfill('0');

	while (p = p->parent() ) {
		result << std::setw(2) << static_cast<unsigned>(p->click().x);
		result << std::setw(2) << static_cast<unsigned>(p->click().y);
	}

	path = result.str();
}

#if 0
std::list<Board> Simulation::run(Board& b, std::list<std::pair<int,int>> path) {
	state state(b);
	std::cout << (std::string)state.board << std::endl;
	for( auto click: path) {
		state.click(click);
		std::cout << (std::string)state.board << std::endl;
	}

	return std::list<Board>();
#if 0
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
			std::cout << (std::string)test.board << std::endl;

			if( test.board.empty() ) {
				// SOLUTION
				return test.clicks;
			} else {
				queue.push(test);
			}
		}
	}

	throw std::logic_error("Invalid State");
#endif
}
#endif