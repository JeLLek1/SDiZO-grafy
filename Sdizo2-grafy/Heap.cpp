#include "Heap.h"
#include "Heap.h"
#include "Menu.h"
#include <iostream>
#include <string>
#include <conio.h>
#include <time.h>

//maksymalna wielko�� tablicy na kopiec jak argument
Heap::Heap(size_t maxSize)
{
	this->maxSize = maxSize;
	this->heap = new Edge *[maxSize];
	this->size = 0;
}

/*
 * Zwraca rodzica na podstawie indeksu
 */
size_t Heap::getParent(size_t children)
{
	return (children - 1) / 2;
}

/*
 * Zwraca lewe dziecko na podstawie indeksu
 */
size_t Heap::getChildLeft(size_t parent)
{
	return parent * 2 + 1;
}

/*
 * Naprawa w d� od wybranego indeksu
 */
void Heap::repairDown(size_t parent)
{
	size_t largestChild = this->getChildLeft(parent);
	while (largestChild < this->size) {
		if (largestChild + 1 < this->size && this->heap[largestChild]->weight > this->heap[largestChild + 1]->weight) {
			largestChild++;
		}
		if (this->heap[parent]->weight > this->heap[largestChild]->weight) {
			Edge* tmp = this->heap[parent];
			this->heap[parent] = this->heap[largestChild];
			this->heap[largestChild] = tmp;
			parent = largestChild;
			largestChild = this->getChildLeft(parent);
		}
		else {
			largestChild = this->size;
		}
	}
}
/*
 * Naprawa w g�r� od wybranego indeksu
 */
void Heap::repairUp(size_t index)
{
	while (index != 0 && this->heap[Heap::getParent(index)]->weight > this->heap[index]->weight) {
		Edge* tmp = this->heap[Heap::getParent(index)];
		this->heap[Heap::getParent(index)] = this->heap[index];
		this->heap[index] = tmp;
		index = Heap::getParent(index);
	}
}

/*
 * Wstawianie nowego elementu wraz z napraw� w g�r�
 */
bool Heap::push(Edge* value)
{
	if (this->size < this->maxSize) {
		this->heap[this->size] = value;
		this->repairUp(this->size);
		this->size++;
		return true;
	}
	return false;
}

/*
 * Usuwanie korzenia. Po usuni�ciu wystarczy naprawa w d�
 */
Edge* Heap::popRoot()
{
	if (this->size > 0) {
		this->size--;
		Edge* nodeReturn = this->heap[0];
		this->heap[0] = this->heap[this->size];
		this->repairDown(0);
		return nodeReturn;
	}
	return nullptr;
}

/*
 * Naprawa w d� dopuki nie dojdzie do korzenia od ostatniego nie li�cia
 */
void Heap::floydConvert()
{
	for (size_t i = this->size / 2; i > 0; i--) {
		this->repairDown(i - 1);
	}
}
//Sprawdzanie czy podana kraw�d� jest w kopcu
bool Heap::findEdge(size_t nodeFrom, size_t nodeTo)
{
	for (size_t i = 0; i < this->size; i++) {
		if (this->heap[i]->nodeFrom == nodeFrom && this->heap[i]->nodeTo == nodeTo) {
			return true;
		}
	}
	return false;
}


//Czysczenie struktury (tu wystarczy pod� 0 jako wielko��)
void Heap::erase()
{
	this->size = 0;
}

/**
 * Destruktor klasy Heap usuwaj�cy wszystkie elementy w strukturze. Usuwa ca�� tablic� kopca
 */
Heap::~Heap()
{
	delete[] this->heap;
}
