#pragma once
//template
template <class T>
struct ListSegment {
	//Warto�
	T value;
	//wska�niki na kolejny i poprzedni element
	ListSegment *prev, *next;
};
//template
template <class T>
class List
{
private:
	//g�owa i ogon listy
	ListSegment<T> *tail, *head;
	//iterator do operacji na kolejnych elementach listy
	ListSegment<T> *iterator;
	
	//Opis metod w pliku implementacji
	ListSegment<T> *search(size_t index);
	
public:
	//ilo�� element�w struktury
	size_t size;
	List();
	T searchEl(size_t index);
	T popFront();
	T popBack();
	T pop(size_t index);
	void pushFront(T value);
	void pushBack(T value);
	bool push(T value, size_t index);
	T getNext();
	T getPrev();
	//pobiera g�ow�
	ListSegment<T>* getHead();
	void setItHead();
	void setItTail();
	void erase();
	~List();
};

#include "List.tpp"