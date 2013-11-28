#ifndef __TIMER_H__
#define __TIMER_H__

#include <windows.h>

class timer {
private:
	LARGE_INTEGER start;
	static LARGE_INTEGER frequenzy;
public:
	timer();
	double get();
};

#endif