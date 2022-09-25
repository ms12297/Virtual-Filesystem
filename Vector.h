#pragma once

#include<iostream>
#include<cstdlib>
#include<iomanip>
#include <stdexcept>
#include<sstream>


using namespace std;
template <typename T>
class MyVector
{
private:
	T* data;						//pointer to int(array) to store elements
	int v_size;						//current size of vector (number of elements in vector)
	int v_capacity;					//capacity of vector
	friend class PositionList;
	friend class Tree;
public:
	MyVector() {
		v_size = 0;
		v_capacity = 0;
		data = new T[v_capacity];
	}//No argument constructor

	MyVector(int cap) {
		v_size = 0;
		v_capacity = cap;
		data = new T[v_capacity];
	}//One Argument Constructor

	MyVector(MyVector& other) {
		v_size = other.size();
		v_capacity = other.capacity();
		data = new T[v_capacity];
		for (int i = 0; i < v_size; i++) {
			insert(i, other.at(i)); //++ to size
			v_size--;
		}
		if (other.size() == other.capacity()) {
			shrink_to_fit();
		}
	}//Copy Constructor

	~MyVector() {
		delete[] data;
	}//Destructor

	void push_back(T element) {
		if (v_size == v_capacity) {
			reserve(max(1, 2 * v_capacity));
		}
		data[v_size] = element;
		v_size++;
	}//Add an element at the end of vector

	void insert(int index, T element) {
		if (index < 0 || index >= v_size) throw out_of_range("Vector index is out of range...!");
		else {
			if (v_size == v_capacity)
				reserve(max(1, 2 * v_capacity));
			for (int j = v_size - 1; j >= index; j--) {
				data[j + 1] = data[j];
			}
			data[index] = element;
			v_size++;
		}

	}//Add an element at the index 

	void erase(int index) {
		if (index < 0 || index >= v_size) throw out_of_range("Vector index is out of range...!");
		else {
			for (int j = index + 1; j <= v_size - 1; j++)
				data[j - 1] = data[j];
			v_size--;
		}

	}//Removes an element from the index

	T& operator[](int index) {
		return data[index];
	}		//return reference of the element at index

	T& at(int index) {
		if (index < 0 || index >= v_size) throw out_of_range("Vector index is out of range...!");
		else return data[index];
	}//return reference of the element at index

	const T& front() {
		if (!empty()) {
			return data[0];
		}
		else throw invalid_argument("Vector is empty!");
	}//Returns reference of the first element in the vector

	const T& back() {
		if (!empty()) {
			return data[v_size - 1];
		}
		else throw invalid_argument("Vector is empty!");
	}//Returns reference of the Last element in the vector

	int size() const {
		return v_size;
	}				//Return current size of vector
	int capacity() const {
		return v_capacity;
	}//Return capacity of vector

	bool empty() const {
		return (v_size == 0);
	} //Rturn true if the vector is empty, False otherwise

	void shrink_to_fit() {
		if (v_size == v_capacity) return;
		else {
			v_capacity = v_size - 1; // to allow the reserve function to continue
			reserve(v_size);
		}
	}//Reduce vector capacity to fit its size

	void display();

	void reserve(int newcap) {
		if (v_capacity >= newcap) return;
		T* newarr = new T[newcap];
		for (int i = 0; i < v_size; i++) // copy contents to new array
			newarr[i] = data[i];
		if (data != NULL) delete[] data; // discard old array
		data = newarr;
		v_capacity = newcap;
	}
};



