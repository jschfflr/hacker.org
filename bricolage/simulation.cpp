#include "simulation.h"
#include <iostream>
#include <windows.h>
#include <process.h>

#include "stats.h"
#include "monitor.h"

#ifdef _DEBUG   
#ifndef DBG_NEW      
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )      
#define new DBG_NEW   
#endif
#endif  // _DEBUG


template<class T>
class StackSize : public Variable {
	std::stack<T>* stack;
public:
	StackSize(std::stack<T>* stack) {
		this->stack = stack;
	}

	std::string get() {
		std::stringstream stream;
		stream << stack->size();
		return stream.str();
	}
};

Simulation::Simulation(Board& board):
	stack(256){
	state start(board);
	stack.push(start);
}

Simulation::~Simulation() {
	//StatsManager::Get().RemoveVar("stack");
}

void Simulation::resolve(std::list<std::pair<int, int>> path) {
	if ( running ) 
		this->path = path;
	running = false;
}

void Simulation::thread(Simulation* self) {
	state state;
	while (self->running) {
		self->stack_lock.lock();
			//monitor::_emit(monitor::event("stack") << self->stack.size());
			
		::state* p = self->stack.pop();
		if ( !p ) {
			self->stack_lock.unlock();
			Sleep(10);
			continue;
		}
		else {
			state = *p;
			self->stack_lock.unlock();
		}
		
#ifdef _DEBUG
		//monitor::_emit(monitor::event("state") << state.board.width() << state.board.height() << state.board.serialize() << std::thread::id() << state.path());
#endif
		auto areas = state.board.areas();

		if (areas.size() == 0 && !state.board.empty())
			continue; // 

		for (auto it = areas.begin(); it != areas.end(); it++) {
			if (it->size() < 3)
				continue;

			::state test = ::state(state);
			test.click(*it);

			if (test.board.empty()) {
				// SOLUTION
				self->resolve(test.clicks);
				return;
			}
			else {
				self->stack_lock.lock();
					self->stack.push(test);
				self->stack_lock.unlock();
			}
		}
	}
}

std::list<std::pair<int,int>> Simulation::run() {
	running = true;
	for (unsigned int i = 0; i < std::thread::hardware_concurrency(); i++)
		threads.push_back(std::thread(thread, this));

	for (auto it = threads.begin(); it != threads.end(); it++)
		it->join();
	
	return path;
}


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