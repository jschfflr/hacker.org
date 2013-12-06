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

	void emit(const state* from, const point* by, const state* to);
	void release();

	static void _emit(const state* from, const point* by, const state* to);
	static graph* create(std::string filename);

private:
	static graph* instance;
};

#ifdef _DEBUG
	#define GRAPH(from, by, to) graph::_emit(from,by,to);
#else
	#define GRAPH
#endif

#endif