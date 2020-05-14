#include "List.h"
#pragma once
//plik tpp dla oddzielenia implementacji od definicji (tak naprawdê klasa template ca³a jest definicj¹)

//Wyszukiwanie elementu o podanym indeksie
template<class T>
ListSegment<T>* List<T>::search(size_t index)
{
	ListSegment<T>* tmp;
	//szukanie albo od przodu albo od ty³u w zale¿noœci od op³acalnoœci
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
	//je¿eli jest element o takim indeksie to go zwróci, je¿eli nie, to wynik pusty
	if (index >= 0 && index < this->size) {
		return this->search(index)->value;
	}
	return T();
}

//Usuñ element z przodu
template<class T>
T List<T>::popFront()
{
	//je¿eli pocz¹tek nie jest nullem
	if (this->head != nullptr) {
		ListSegment<T>* tmp = this->head;
		//przechowanie elementu aby go zwróciæ
		T data = tmp->value;
		//Naprawianie po³¹czeñ dla reszty elementów
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

//Usuñ element z ty³u
template<class T>
T List<T>::popBack()
{
	if (this->tail != nullptr) {
		ListSegment<T>* tmp = this->tail;
		//przechowanie elementu aby go zwróciæ
		T data = tmp->value;
		//Naprawianie po³¹czeñ dla reszty elementów
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

//Usuñ element o podanym indeksie
template<class T>
T List<T>::pop(size_t index)
{
	//je¿eli index 0 to nale¿y usun¹æ z przodu
	if (index == 0) {
		return this->popFront();
	}
	//je¿eli index to ostatni element to nalezy usun¹æ z ty³u
	if (index == this->size - 1) {
		return this->popBack();
	}
	//je¿eli miêdzy tamtymi dwoma to nale¿y usun¹æ ze œrodka
	if (index > 0 && index < this->size - 1) {
		//znajdowanie elementu o podanym indeksie do usuniêcia
		ListSegment<T>* tmp = this->search(index);
		//wartoœæ do zwrócienia
		T data = tmp->value;
		//naprawa po³¹czeñ
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

//Dodaj element z ty³u
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
	//je¿eli indeks 0 to nale¿y wstawi¹ na pocz¹tku
	if (index == 0) {
		this->pushFront(value);
		return true;
	}
	//je¿eli indeks wiêkszy ni¿ ostatni indeks w liœcie to na koñcu
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

//zwróc wartoœæ elementu nastêpnego od iteratora
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

//zwróc element poprzedni od iteratora
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

//ustaw iterator na przód
template<class T>
void List<T>::setItHead()
{
	this->iterator = this->head;
}

//ustaw iterator na ty³
template<class T>
void List<T>::setItTail()
{
	this->iterator = this->tail;
}


//Wyczyœæ strutkurê (uwaga, elementy zaalokowane dynamicznie poza struktur¹ nie zostan¹ w niej usuniête)
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
//Destruktor nie zadba o usuniêcie dynamicznie stworzonych wartoœci przechowywanych w liœcie
template<class T>
inline List<T>::~List()
{
	this->erase();
}