#include "stats.h"

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
		auto cb = self->vars["stack"];
		if (cb)
			std::cout << "--" << cb->get().c_str() << std::endl;
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

