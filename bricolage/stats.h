#ifndef __STATS_H__
#define __STATS_H__

#include <map>
#include <mutex>
#include <sstream>
#include <iostream>
#include <windows.h>

#include <iostream>


class Variable {
public:
	virtual ~Variable(){}
	virtual std::string get() = 0;
};


template<typename T>
class StaticVariable : public Variable {
	T* ref;

public:
	StaticVariable(T* ref) {
		this->ref = ref;
	}

	std::string get() {
		std::stringstream stream;
		stream << *this->ref;
		return stream.str();
	}
};

template<class T>
class DynamicVariable : public Variable {
	T (*callback)(void);

public:
	DynamicVariable(T(*callback)(void)) {
		this->callback = callback;
	}

	std::string get() {
		std::stringstream stream;
		stream << callback();
		return stream.str();
	}
};


class StatsManager {

	std::mutex lock;
	std::map<std::string, Variable*> vars;
	HANDLE thread;

public:
	StatsManager();
	void AddVar(std::string name, Variable*);
	void StatsManager::RemoveVar(std::string name);

private:
	static DWORD WINAPI collector(void* p);
	static StatsManager& instance;

public:
	static StatsManager& Get();
};

#endif