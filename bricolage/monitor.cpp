#include "monitor.h"

#include <iostream>

#ifdef _DEBUG   
#ifndef DBG_NEW      
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )      
#define new DBG_NEW   
#endif
#endif  // _DEBUG

monitor::monitor(std::string filename) {
	stream.open(filename);
}

monitor::~monitor() {
	stream.close();
}

void monitor::emit(const event& event) {
	mutex.lock();
		stream << timer.millis() << "\t" << (std::string)event << std::endl;
		std::cout << timer.millis() << "\t" << (std::string)event << std::endl;
	mutex.release();
}

void monitor::release() {
	delete this;
}

void monitor::_emit(const event& event) {
	instance->emit(event);
}

monitor* monitor::create(std::string filename) {
	instance = new monitor(filename);
	return instance;
}

monitor* monitor::instance;