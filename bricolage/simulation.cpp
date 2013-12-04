#include "common.h"
#include "simulation.h"
#include <iostream>
#include <windows.h>
#include <process.h>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include "stats.h"
#include "monitor.h"


#include "graph.h"
#include "stack.h"

simulation::simulation(const board& board) {
	samples = 0;
	possibilities = 0;

	initial = new state(board);
}

simulation::~simulation() {
	delete initial;
}

void simulation::resolve(const state* state) {
	bool process = false;
	lock.lock();
	if (running) {
		this->result = state;
		process = true;
	}
	

	running = false;
	lock.release();

	if (process) {
		stack<point> stack(1024);
		const ::state* p = this->result;
		while (p->parent()) {
			stack.push(p->click());
			p = p->parent();
		}

		point c;
		std::stringstream result;
		result << std::hex << std::setfill('0');
		while ( stack.pop(&c) ) {
			result << std::setw(2) << static_cast<unsigned>(c.x);
			result << std::setw(2) << static_cast<unsigned>(c.y);
		}
		path = result.str();
	}
}

void simulation::dfs_wrapper(simulation* self, state* initial) {
	try {
		self->depth_first_search(initial);
	}
	catch (std::exception e) {
		monitor::_emit(monitor::event("exception") << "thread" << e.what());
		terminate();
	}
}

void simulation::depth_first_search(state* initial) {
	::stack<state*> stack(initial->board()->width() * initial->board()->height() << 12);
	stack.push(initial);
	state* s;
	while (running && stack.top(&s)) {
		samples++;
		possibilities += s->board()->areas()->size();
		if (s->board()->empty()) {
			resolve(s);
			break; // clean up the mess
		}
		
		area* a;
		if (s->board()->areas()->pop(&a)) {
			if (a->size() < 3) {
				delete a;
				continue;
			}
				

			state* p = new state(s, *a);
			graph::_emit(s, p);
			stack.push(p);
			delete a;
			continue;
		}

		stack.pop();
		delete s;
	}

	while(stack.pop(&s))
		delete s;
}

void simulation::dfs(std::string* path) {

	area* a;
	timer t;
	
	running = true;
	while (initial->board()->areas()->pop(&a)) {
		if (a->size() < 3){
			delete a;
			continue;
		}
			
		state* s = new state(initial, *a);
		graph::_emit(initial, s);
		threads.push_back(std::thread(dfs_wrapper, this, s));
		delete a;
	}

	for (auto it = threads.begin(); it != threads.end(); it++)
		it->join();

	
	monitor::_emit(monitor::event("samples") << samples << t.get());
	monitor::_emit(monitor::event("possibilities") << possibilities << t.get());
	*path = this->path;
}

#if 0
void simulation::thread_wrapper(simulation* self) {
	try {
		self->thread();
	}
	catch (std::exception e) {
		monitor::_emit(monitor::event("exception") << "thread" << e.what());
		terminate();
	}
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
			heap.pop(&state);
			samples++;
			possibilities += state->board()->areas()->size();
			heap_lock.unlock();
		}

		if (state->board()->areas()->size() == 0 && !state->board()->empty()) {
			continue; // 
		}

		area area;
		while (state->board()->areas()->pop(&area)) {
			// Can not click this area
			if (area.size() < 3)
				continue;

			// perform click
			::state *p = new ::state(state, area);
			
			if (p->board()->empty()) {
				return resolve(p);
			}
			else {
				heap_lock.lock();
				heap.insert(p);
				heap_lock.unlock();
			}
		}
	}
}

void simulation::run(std::string* path) {
	running = true;
	timer t;
	for (unsigned int i = 0; i < 1 /* std::thread::hardware_concurrency() */; i++)
		threads.push_back(std::thread(thread_wrapper, this));

	for (auto it = threads.begin(); it != threads.end(); it++)
		it->join();

	monitor::_emit(monitor::event("samples") << samples << t.get());
	monitor::_emit(monitor::event("possibilities") << possibilities << t.get());

	std::stringstream result;
	const state* p = this->result;
	result << std::hex << std::setfill('0');

	while (p->parent()) {
		result << std::setw(2) << static_cast<unsigned>(p->click().x);
		result << std::setw(2) << static_cast<unsigned>(p->click().y);
		p = p->parent();
	}
	*path = result.str();
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
#endif