#pragma once
#include "Node.h"
class Heap
{
private:
	size_t maxSize;
	//Tablica wskaŸników na node (kluczem bêdzie waga)
	Edge** heap;
	size_t size;
	size_t getParent(size_t children);
	size_t getChildLeft(size_t parent);
	void repairDown(size_t index);
	void repairUp(size_t index);
public:
	Heap(size_t maxSize);
	bool push(Edge* node);
	void erase();
	Edge* popRoot();
	void floydConvert();
	bool findEdge(size_t nodeFrom, size_t nodeTo);
	~Heap();
};

