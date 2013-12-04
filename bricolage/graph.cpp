#include "graph.h"

#include <iostream>

#ifdef _DEBUG   
#ifndef DBG_NEW      
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )      
#define new DBG_NEW   
#endif
#endif  // _DEBUG

graph::graph(std::string filename) {
	stream.open(filename);
	stream << "digraph {" << std::endl;
}

graph::~graph() {
	stream << "}" << std::endl;
	stream.close();
}

void graph::emit(state* from, state* to) {
	return;
	mutex.lock();
	stream << "\t" << from->board()->debug() << " -> " << to->board()->debug() << std::endl;
	mutex.release();
}

void graph::release() {
	delete this;
}

void graph::_emit(state* from, state* to) {
	instance->emit(from, to);
}

graph* graph::create(std::string filename) {
	instance = new graph(filename);
	return instance;
}

graph* graph::instance = 0;