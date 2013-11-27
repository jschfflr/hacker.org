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

struct param {
	int id;
	Simulation* pSimulation;
	std::list<std::pair<int, int>> path;

	param() {
		id = 0;
		pSimulation = 0;
	}

	param(Simulation* pSimulation) {
		this->pSimulation = pSimulation;
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

DWORD WINAPI Simulation::thread(void* pContext) {
	param* pParam = static_cast <param*>(pContext);
	Simulation* pSimulation = pParam->pSimulation;
	int i = 0;
	std::string id = "i";
	char buffer[10];
	_itoa_s(pParam->id, &buffer[0], 10, 10);
	id += buffer;

	StatsManager::Get().AddVar(id, new StaticVariable<int>(&i));
	State state;
	while (true) {
		if (pSimulation->running == false)
			break;
		i++;
		pSimulation->stack_lock.lock();

		if (pSimulation->stack.empty()) {
			pSimulation->stack_lock.unlock();
			Sleep(100); 
		}
		else {
			state = (State)pSimulation->stack.top();
			pSimulation->stack.pop();
			pSimulation->stack_lock.unlock();
		}
		
		

		auto areas = state.board.areas();

		// fetch all available areas from board
		// if len(areas) == 0 and not empty:
		//	invalid state -> don't process
		//  continue
		//
		if (areas.size() == 0 && !state.board.empty())
			continue; // 

		// for each area
		//		simulate click on area
		//		if empty:
		//			SOLUTION FOUND!!!
		//          return its path
		//		else:
		//			append to queue
		//
		for (auto it = areas.begin(); it != areas.end(); it++) {
			if (it->size() < 3)
				continue;

			State test = State(state);
			test.click(*it);

			if (test.board.empty()) {
				// SOLUTION
				pParam->path = test.clicks;
				return 1;
			}
			else {
				pSimulation->stack_lock.lock();
				pSimulation->stack.push(test);
				pSimulation->stack_lock.unlock();
			}
		}
	}

	return 0;
}

std::list<std::pair<int,int>> Simulation::run() {
	param p1(this);

	SYSTEM_INFO sysinfo;
	GetSystemInfo(&sysinfo);
	int cpus = sysinfo.dwNumberOfProcessors;
	HANDLE *handles = new HANDLE[cpus];
	param *params = new param[cpus];


	running = true;
	for (int i = 0; i < cpus; i++) {
		params[i].id = i;
		params[i].pSimulation = this;
		handles[i] = CreateThread(NULL, 0, Simulation::thread, &params[i], 0, NULL);
	}
	
	int i = WaitForMultipleObjects(cpus, handles, FALSE, INFINITE) - WAIT_OBJECT_0;
	running = false;

	HANDLE *tmp = new HANDLE[cpus - 1];
	for (int a = 0,b = 0; a < cpus; a++) {
		if (a == i)
			continue;
		tmp[b++] = handles[a];
	}
	
	WaitForMultipleObjects(cpus, handles, FALSE, INFINITE);
	 
	std::list<std::pair<int, int>> path = params[i].path;
	
	delete[] tmp;
	delete[] params;
	delete[] handles;
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