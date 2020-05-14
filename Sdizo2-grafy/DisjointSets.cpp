#include "DisjointSets.h"

DisjointSets::DisjointSets(size_t length)
{
	//inicjalizacja tablicy rodzic�w
	this->parent = new size_t[length];
	//inicjalizacja tablicy rang
	this->rank = new size_t[length];
	for (size_t i = 0; i < length; i++) {
		//pocz�tkowo ka�dy wierzcho�ek jest oddzielony od siebie (jest sam sobie rodzicem)
		this->parent[i] = i;
		//pocz�tkowo ka�dy wierzcho�ek ma range 1 bo zbi�r ma 1 element
		this->rank[i] = 0;
	}
}

size_t DisjointSets::findSet(size_t x)
{
	if (this->parent[x] != x) {
		parent[x] = this->findSet(parent[x]);
	}
	return parent[x];
}

void DisjointSets::unionSets(size_t x, size_t y)
{
	//znajdowanie najwy�szego rodzica x
	x = this->findSet(x);
	//znajdowanie najwy�szego rodzica y
	y = this->findSet(y);

	//je�eli ranga x jest mniejsza to rodzicem x b�dzie y - nie zmieni to rangi najwy�szego elementu
	if (this->rank[x] < this->rank[y]) {
		this->parent[x] = y;
	}
	else {
		//w przeciwnym wypadku to rodzicem y b�dzie x
		this->parent[y] = x;
	}
	//je�eli mia�y tak� sam� rang� to teraz trzeba zwi�kszy� rang� x bo sta� si� o poziom wi�kszy
	if (rank[x] == rank[y]) {
		rank[x] += 1;
	}
}

DisjointSets::~DisjointSets()
{
	delete[] this->parent;
}
