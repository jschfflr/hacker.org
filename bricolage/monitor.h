#ifndef __MONITOR_H__
#define __MONITOR_H__

#include <string>
#include <sstream>
#include <fstream>

#include "common.h"
#include "timer.h"
#include "mutex.h"

class monitor {
public:
	class event {
		std::string name;
		std::stringstream value;

	public:
		event(std::string name) {
			this->name = name;
		}

		template<typename T>
		event& operator <<(const T& value) {
			this->value << "\t";
			this->value << value;
			return *this;
		}

		operator std::string() const {
			std::string ret = name;
			ret += value.str();
			return ret;
		}
	};

private:
	timer timer;
	mutex mutex;
	std::ofstream stream;

public:
	monitor(std::string filename);
	~monitor();

	void emit(const event&);
	void release();

	static void _emit(const event&);
	static monitor* create(std::string filename);
private:
	static monitor* instance;
};



#endif