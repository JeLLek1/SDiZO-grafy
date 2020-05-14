#include "DisjointSets.h"

DisjointSets::DisjointSets(size_t length)
{
	//inicjalizacja tablicy rodziców
	this->parent = new size_t[length];
	//inicjalizacja tablicy rang
	this->rank = new size_t[length];
	for (size_t i = 0; i < length; i++) {
		//pocz¹tkowo ka¿dy wierzcho³ek jest oddzielony od siebie (jest sam sobie rodzicem)
		this->parent[i] = i;
		//pocz¹tkowo ka¿dy wierzcho³ek ma range 1 bo zbiór ma 1 element
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
	//znajdowanie najwy¿szego rodzica x
	x = this->findSet(x);
	//znajdowanie najwy¿szego rodzica y
	y = this->findSet(y);

	//je¿eli ranga x jest mniejsza to rodzicem x bêdzie y - nie zmieni to rangi najwy¿szego elementu
	if (this->rank[x] < this->rank[y]) {
		this->parent[x] = y;
	}
	else {
		//w przeciwnym wypadku to rodzicem y bêdzie x
		this->parent[y] = x;
	}
	//je¿eli mia³y tak¹ sam¹ rangê to teraz trzeba zwiêkszyæ rangê x bo sta³ siê o poziom wiêkszy
	if (rank[x] == rank[y]) {
		rank[x] += 1;
	}
}

DisjointSets::~DisjointSets()
{
	delete[] this->parent;
}
