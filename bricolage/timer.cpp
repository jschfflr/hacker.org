#include "timer.h"

timer::timer() {
	QueryPerformanceCounter(&start);

	if( frequenzy.QuadPart == 0 )
		QueryPerformanceFrequency(&frequenzy);
}

double timer::get() {
	LARGE_INTEGER now;
	QueryPerformanceCounter(&now);
	return (now.QuadPart - start.QuadPart) / double(frequenzy.QuadPart);
}

LARGE_INTEGER timer::frequenzy = {0};
