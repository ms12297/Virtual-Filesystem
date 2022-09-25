#pragma once
#include <iostream>

using namespace std;

template <typename E>
class Node
{
private:
	E elem; //data element 
	Node* next; //Link (pointer) to the next Node

public:
	Node(E elem) : elem(elem), next(NULL)
	{}
	template<typename E>friend class SLinkedList;
};
//=====================================
template <typename E>
class SLinkedList
{
private:
	Node<E>* head; // pointer to the head of list
public:
	SLinkedList() {
		this->head = NULL;
	}
	~SLinkedList() {
		while (!empty()) removeFront();
	}
	bool empty() const {
		if (head == NULL)
			return true;

		return false;
	}
	void addFront(E elem) {
		Node<E>* v = new Node<E>(elem);
		v->next = head;
		head = v;
	}
	void removeFront() {
		if (!empty()) {
			Node<E>* old = head;
			head = old->next;
			delete old;
		}
		else
			cout << "List is empty!\n" << endl;
	}
	E& front() {
		return head->elem;
	}
};

template <typename E>
class CStack {
private:
	SLinkedList<E> S; // linked list
	int n; // number of elements in stack

public:
	CStack() : S(), n(0) { }  //constructor
	int size() const {
		return n;
	}
	bool empty() const {
		return (n == 0);
	}
	const E& top() {
		if (empty()) cout << "List is empty!\n";
		return S.front();
	}
	void push(const E& e) {
		++n;
		S.addFront(e);
	}
	void pop() {
		if (empty()) cout << "List is empty!\n";
		--n;
		S.removeFront();
	}
};