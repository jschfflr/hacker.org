#include "stats.h"
#include <Psapi.h>

StatsManager::StatsManager() {
	thread = CreateThread(NULL, 0, StatsManager::collector, this, 0, NULL);
}

void StatsManager::AddVar(std::string name, Variable* pVar) {
	lock.lock();
		auto pair = std::pair<std::string, Variable*>(name, pVar);
		vars.erase(name);
		vars.insert(pair);
	lock.unlock();
}

void StatsManager::RemoveVar(std::string name) {
	lock.lock();
		vars.erase(name);
	lock.unlock();
}

DWORD WINAPI StatsManager::collector(void* p) {
	StatsManager* self = static_cast<StatsManager*>(p);

	while (true) {
		self->lock.lock();

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
		
		std::cout << std::endl;
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

	return 0;
}

StatsManager& StatsManager::Get() {
	return StatsManager::instance;
}

StatsManager& StatsManager::instance = StatsManager();

