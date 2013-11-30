#include "timer.h"

#ifdef _DEBUG   
#ifndef DBG_NEW      
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )      
#define new DBG_NEW   
#endif
#endif  // _DEBUG

timer::timer() {
	QueryPerformanceCounter(&start);

	if( frequenzy.QuadPart == 0 )
		QueryPerformanceFrequency(&frequenzy);
}

double timer::get() const {
	LARGE_INTEGER now;
	QueryPerformanceCounter(&now);
	return (now.QuadPart - start.QuadPart) / double(frequenzy.QuadPart);
}

unsigned long timer::millis() const {
	return static_cast<unsigned long>(get() * 1000 );
}

LARGE_INTEGER timer::frequenzy = {0};
