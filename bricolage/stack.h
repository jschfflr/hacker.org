#ifndef __STACK_H__
#define __STACK_H__

template<class T>
class stack {
	T* _stack;
	int _top;
	int _size;

public:
	stack(size_t size) {
		_top = -1;
		_size = size;
		_stack = new T[size];
	}
	~stack() {
		delete[] _stack;
	}
	
	void push(T& value) {
		if (_top == _size - 1)
			throw std::runtime_error("Stackoverflow");
		_stack[++_top] = T(value);
	}

	T top() {
		return T(_stack[_top]);
	}

	void pop() {
		if (_top == -1)
			return;
		_top--;
	}

	inline size_t size() const { return _top + 1; }
	inline bool empty() const { return _top == -1; }
};

#endif