#include "monitor.h"

#include <iostream>

monitor::monitor(std::string filename) {
	stream.open(filename);
}

monitor::~monitor() {
	stream.close();
}

void monitor::emit(const event& event) {
	mutex.lock();
	stream << timer.millis() << "\t" << (std::string)event << std::endl;
	mutex.release();
}

void monitor::_emit(const event& event) {
	instance->emit(event);
}

monitor* monitor::create(std::string filename) {
	instance = new monitor(filename);
	return instance;
}

monitor* monitor::instance;