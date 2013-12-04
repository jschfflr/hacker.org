#ifndef __GRAPH_H__
#define __GRAPH_H__

#include <fstream>

#include "state.h"
#include "mutex.h"

class graph {
public:
	mutex mutex;
	std::ofstream stream;

public:
	graph(std::string filename);
	~graph();

	void emit(state* from, state* to);
	void release();

	static void _emit(state* from, state* to);
	static graph* create(std::string filename);

private:
	static graph* instance;
};

#endif