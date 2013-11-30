#include "timer.h"

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
