#include "object.h"

#include <atomic>

std::atomic<long long> count;

Object::Object() {
	count.fetch_add(1);
}

Object::Object(const Object&) {
	count.fetch_add(1);
}

Object::~Object(){
	count.fetch_sub(1);
}