#ifndef __OBJECT_H__
#define __OBJECT_H__

#include <atomic>

extern std::atomic<long long> count;

class Object {
protected:
	Object();
	Object::Object(const Object&);
	virtual ~Object();
};

#endif