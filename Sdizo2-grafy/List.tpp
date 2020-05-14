#include "List.h"
#pragma once
//plik tpp dla oddzielenia implementacji od definicji (tak naprawd� klasa template ca�a jest definicj�)

//Wyszukiwanie elementu o podanym indeksie
template<class T>
ListSegment<T>* List<T>::search(size_t index)
{
	ListSegment<T>* tmp;
	//szukanie albo od przodu albo od ty�u w zale�no�ci od op�acalno�ci
	if (this->size / 2 > index) {
		tmp = this->head;
		while (index > 0) {
			tmp = tmp->next;
			index--;
		}
	}
	else {
		tmp = this->tail;
		index = this->size - index;
		while (index > 1) {
			tmp = tmp->prev;
			index--;
		}
	}
	return tmp;
}

//Konstruktor
template<class T>
inline List<T>::List()
{
	this->tail = nullptr;
	this->head = nullptr;
	this->size = 0;
	this->iterator = nullptr;
}

template<class T>
T List<T>::searchEl(size_t index)
{
	//je�eli jest element o takim indeksie to go zwr�ci, je�eli nie, to wynik pusty
	if (index >= 0 && index < this->size) {
		return this->search(index)->value;
	}
	return T();
}

//Usu� element z przodu
template<class T>
T List<T>::popFront()
{
	//je�eli pocz�tek nie jest nullem
	if (this->head != nullptr) {
		ListSegment<T>* tmp = this->head;
		//przechowanie elementu aby go zwr�ci�
		T data = tmp->value;
		//Naprawianie po��cze� dla reszty element�w
		this->head = this->head->next;
		if (this->head != nullptr) {
			this->head->prev = nullptr;
		}
		if (this->head == nullptr || this->head->next == nullptr) {
			this->tail = this->head;
		}
		delete tmp;
		this->size--;
		this->iterator = nullptr;
		return data;
	}
	return T();
}

//Usu� element z ty�u
template<class T>
T List<T>::popBack()
{
	if (this->tail != nullptr) {
		ListSegment<T>* tmp = this->tail;
		//przechowanie elementu aby go zwr�ci�
		T data = tmp->value;
		//Naprawianie po��cze� dla reszty element�w
		this->tail = this->tail->prev;
		if (this->tail != nullptr) {
			this->tail->next = nullptr;
		}
		if (this->tail == nullptr || this->tail->prev == nullptr) {
			this->head = this->tail;
		}
		delete tmp;
		this->size--;
		this->iterator = nullptr;
		return data;
	}
	return T();
}

//Usu� element o podanym indeksie
template<class T>
T List<T>::pop(size_t index)
{
	//je�eli index 0 to nale�y usun�� z przodu
	if (index == 0) {
		return this->popFront();
	}
	//je�eli index to ostatni element to nalezy usun�� z ty�u
	if (index == this->size - 1) {
		return this->popBack();
	}
	//je�eli mi�dzy tamtymi dwoma to nale�y usun�� ze �rodka
	if (index > 0 && index < this->size - 1) {
		//znajdowanie elementu o podanym indeksie do usuni�cia
		ListSegment<T>* tmp = this->search(index);
		//warto�� do zwr�cienia
		T data = tmp->value;
		//naprawa po��cze�
		tmp->prev->next = tmp->next;
		tmp->next->prev = tmp->prev;

		delete tmp;
		this->size--;
		this->iterator = nullptr;
		return data;
	}
	return T();
}

//dodaj element z przodu
template<class T>
void List<T>::pushFront(T value)
{
	ListSegment<T>* tmp = new ListSegment<T>();
	tmp->value = value;
	tmp->prev = nullptr;
	tmp->next = this->head;
	if (this->head != nullptr) {
		this->head->prev = tmp;
	}
	else {
		this->tail = tmp;
	}
	this->head = tmp;
	this->size++;
}

//Dodaj element z ty�u
template<class T>
void List<T>::pushBack(T value)
{
	ListSegment<T>* tmp = new ListSegment<T>();
	tmp->value = value;
	tmp->prev = this->tail;
	tmp->next = nullptr;
	if (this->tail != nullptr) {
		this->tail->next = tmp;
	}
	else {
		this->head = tmp;
	}
	this->tail = tmp;
	this->size++;
}

//dodaj element na wybranej pozycji
template<class T>
bool List<T>::push(T value, size_t index)
{
	//je�eli indeks 0 to nale�y wstawi� na pocz�tku
	if (index == 0) {
		this->pushFront(value);
		return true;
	}
	//je�eli indeks wi�kszy ni� ostatni indeks w li�cie to na ko�cu
	if (index == this->size) {
		this->pushBack(value);
		return true;
	}
	if (index > 0 && index < this->size) {
		//w przeciwnym wypadku wstawianie po indeksie
		ListSegment<T>* tmp = new ListSegment<T>();
		tmp->value = value;
		ListSegment<T>* beforeTmp = this->search(index);
		tmp->prev = beforeTmp->prev;
		tmp->next = beforeTmp;
		beforeTmp->prev->next = tmp;
		beforeTmp->prev = tmp;

		this->size++;
		return true;
	}
	return false;
}

//zwr�c warto�� elementu nast�pnego od iteratora
template<class T>
T List<T>::getNext()
{
	if (this->iterator != nullptr) {
		T returnEl = this->iterator->value;
		this->iterator = this->iterator->next;
		return returnEl;
	}
	return T();
}

//zwr�c element poprzedni od iteratora
template<class T>
T List<T>::getPrev()
{
	if (this->iterator != nullptr) {
		T returnEl = this->iterator->value;
		this->iterator = this->iterator->prev;
		return returnEl;
	}
	return T();
}

template<class T>
ListSegment<T>* List<T>::getHead()
{
	return this->head;
}

//ustaw iterator na prz�d
template<class T>
void List<T>::setItHead()
{
	this->iterator = this->head;
}

//ustaw iterator na ty�
template<class T>
void List<T>::setItTail()
{
	this->iterator = this->tail;
}


//Wyczy�� strutkur� (uwaga, elementy zaalokowane dynamicznie poza struktur� nie zostan� w niej usuni�te)
template<class T>
void List<T>::erase()
{
	while (this->head != nullptr) {
		ListSegment<T>* tmp = this->head;
		this->head = this->head->next;
		delete tmp;
	}
	this->size = 0;
	this->tail = nullptr;
	this->iterator = nullptr;
}

//destruktor
//Destruktor nie zadba o usuni�cie dynamicznie stworzonych warto�ci przechowywanych w li�cie
template<class T>
inline List<T>::~List()
{
	this->erase();
}