#include "stats.h"
#include <windows.h>
#include <Psapi.h>

#ifdef _DEBUG   
#ifndef DBG_NEW      
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )      
#define new DBG_NEW   
#endif
#endif  // _DEBUG


StatsManager::StatsManager() {
	thread = std::thread(collector, this);
}

void StatsManager::AddVar(std::string name, Variable* pVar) {
	lock.lock();
		auto pair = std::pair<std::string, Variable*>(name, pVar);
		vars.erase(name);
		vars.insert(pair);
	lock.unlock();
}

Variable* StatsManager::GetVar(std::string name) {
	lock.lock();
		Variable* p = vars[name];
	lock.unlock();
	return p;
}

void StatsManager::RemoveVar(std::string name) {
	lock.lock();
		vars.erase(name);
	lock.unlock();
}

void StatsManager::collector(StatsManager* self) {
	while (true) {
		self->lock.lock();
		/*
		std::cout << GetTickCount();
		std::cout << "\t";

		auto var = self->vars["stack"];
		if (var)
			std::cout << var->get().c_str();
		
		std::cout << "\t";
		
		{
			PROCESS_MEMORY_COUNTERS counters;
			GetProcessMemoryInfo(GetCurrentProcess(), &counters, sizeof(counters));
			std::cout << counters.WorkingSetSize;
		}

		std::cout << "\t";

		auto objects = self->vars["objects"];
		if (objects)
			std::cout << objects->get().c_str();
		*/
		//std::cout << std::endl;
		/*
			//for (auto it = self->vars.begin(); it != self->vars.end(); it++) {
			std::cout << it->first.c_str();
			std::cout << ": ";
			std::cout << it->second->get().c_str();
			std::cout << std::endl;
		}
		*/
		self->lock.unlock();
		Sleep(1000);
	}
}

StatsManager& StatsManager::Get() {
	return StatsManager::instance;
}

StatsManager& StatsManager::instance = StatsManager();

