#ifndef __STATS_H__
#define __STATS_H__

#include <map>
#include <mutex>
#include <thread>
#include <sstream>
#include <iostream>

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
	std::thread thread;

public:
	StatsManager();
	void AddVar(std::string name, Variable*); 
	Variable* GetVar(std::string name);
	void RemoveVar(std::string name);

private:
	static void collector(StatsManager* p);
	static StatsManager& instance;

public:
	static StatsManager& Get();
};

#endif