#ifndef __HEAP_H__
#define __HEAP_H__

#ifdef min
#undef min
#endif

template<class T>
class heap {
	T* _heap;
	size_t _size;
	size_t _capacity;

public:
	heap(size_t capacity) {
		_size = 0;
		_capacity = capacity;
		_heap = new T[capacity];
	}

	~heap() {
		delete[] _heap;
	}

	void insert(const T& element) {
		if (_size == _capacity)
			throw std::runtime_error("Heap is full");
		int i = _size++;
		_heap[i] = element;
		decrease(i);
	}

	T remove(size_t i) {
		T item = _heap[i];
		_heap[i] = T();
		swap(i, --_size);
		if (i != _size) {
			if (i == 0 || _heap[i] < _heap[parent(i)])
				heapify(i);
			else
				decrease(i);
		}

		return item;
	}

	T min() {
		return remove(0);
	}

	inline size_t size() const { return _size; }
	inline bool empty() const { return _size == 0; }
private:
	inline void swap(int a, int b) {
		T tmp = _heap[a];
		_heap[a] = _heap[b];
		_heap[b] = tmp;
	}

	inline void decrease(int i) {
		while (i > 0 && _heap[i] < _heap[parent(i)]) {
			swap(i, parent(i));
			i = parent(i);
		}
	}

	inline void heapify(int a) {
		int i = a;
		do {
			int min = i;
			if (left(i) < _size && _heap[left(i)] < _heap[min])
				min = left(i);
			if (right(i) < _size && _heap[right(i)] < _heap[min])
				min = right(i);
			if (min == i)
				break;
			swap(i, min);
			i = min;
		} while (true);
	}

	inline size_t parent(int i) const { return ( i - 1 ) / 2; }
	inline size_t left(int i) const { return 2 * i + 1; }
	inline size_t right(int i) const { return 2 * i + 2; }

};

#endif