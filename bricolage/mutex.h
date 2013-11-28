#ifndef __MUTEX_H__
#define __MUTEX_H__

#include <atomic>

class mutex {
	int _lock;

public:
	mutex();
	void lock();
	void release();
	void unlock();
	~mutex();
};

#endif