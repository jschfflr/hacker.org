#include "mutex.h"

mutex::mutex() {
	_lock = 0;
}

void mutex::lock() {
	__asm {
_loop:
		mov eax, 0
		mov ebx, 1
		mov ecx, this
		lock cmpxchg [ecx]._lock, ebx
		JNZ _loop
	}
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