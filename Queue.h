#pragma once
#include <iostream>
#include <iomanip>

using namespace std;

template <typename T>
class Queue
{
private:
	T* array;
	int N;	//Array Size (Capacity)
	int r;	//index where a new element will be added
public:
	Queue() {
		this->array = new T[10];
		this->r = 0;
		this->N = 10;
	}

	~Queue() {
		if (array != NULL) {
			delete[] array;
		}
	}

	void enqueue(T elem) {
		if (!isFull()) {
			array[r] = elem;
			r++;
		}
	}

	T dequeue() {
		if (!isEmpty()) {
			T save = array[0];
			for (int i = 1; i < r; i++) {
				array[i - 1] = array[i];
			}
			r--;
			return save;
		}
		else cout << "Queue is empty!\n";
	}

	bool isEmpty() {
		return (r == 0);
	}

	bool isFull() {
		return (r == N);
	}

	T top() {
		return array[0];
	}

};