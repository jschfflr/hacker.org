#ifndef __STACK_H__
#define __STACK_H__

#include "common.h"

template<class T>
class stack {
	T* _stack;
	size_t _top;
	size_t _size;

public:
	stack(size_t size) {
		_top = -1;
		_size = size;
		_stack = new T[size];
	}

	~stack() {
		delete[] _stack;
	}

	void clear() {
		//for (size_t i = 0; i < _size; i++)
		//	delete _stack[i];

		_top = -1;
	}
	
	void push(const T& value) {
		if (_top == _size - 1)
			throw std::runtime_error("Stackoverflow");
		_stack[++_top] = value;
	}

	bool top(T* item) const {
		if (_top == -1)
			return false;

		*item = _stack[_top];
		return true;
	}

	void pop() {
		if (_top == -1)
			return;
		_top--;
	}

	bool pop(T* element) {
		if (_top == -1)
			return false;

		*element = _stack[_top--];
		return true;
	}

	inline size_t size() const { return _top + 1; }
	inline bool empty() const { return _top == -1; }
};

#endif