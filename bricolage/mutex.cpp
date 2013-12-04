#include "mutex.h"


mutex::mutex() {
	_lock = 0;
}

void mutex::lock() {
#ifndef _WIN64
	__asm {
_loop:
		mov eax, 0
		mov ebx, 1
		mov ecx, this
		lock cmpxchg [ecx]._lock, ebx
		JNZ _loop
	}
#else
	_lock = 1; //TODO: How to do this with x64
#endif
}

void mutex::release() {
	_lock = 0;
}

void mutex::unlock() {
	release();	
}

mutex::~mutex() {
	_lock = 0;
}