#include "simulation.h"
#include <iostream>
#include <windows.h>
#include <process.h>

#include "stats.h"

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

Simulation::Simulation(Board& board) {
	State start(board);
	stack.push(start);
	StatsManager::Get().AddVar("stack", new StackSize<State>(&this->stack));
}

Simulation::~Simulation() {
	StatsManager::Get().RemoveVar("stack");
}

void Simulation::resolve(std::list<std::pair<int, int>> path) {
	if ( running ) 
		this->path = path;
	running = false;
}

void Simulation::thread(Simulation* self) {
	State state;
	while (self->running) {
		self->stack_lock.lock();

		if (self->stack.empty()) {
			self->stack_lock.unlock();
			Sleep(100); 
		}
		else {
			state = self->stack.top();
			self->stack.pop();
			self->stack_lock.unlock();
		}
		
		auto areas = state.board.areas();

		if (areas.size() == 0 && !state.board.empty())
			continue; // 

		for (auto it = areas.begin(); it != areas.end(); it++) {
			if (it->size() < 3)
				continue;

			State test = State(state);
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
	State state(b);
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