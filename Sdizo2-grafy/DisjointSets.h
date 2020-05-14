#pragma once
struct DisjointSets
{
	//tablica rodziców
	size_t* parent;
	//tablica rang
	size_t* rank;

	DisjointSets(size_t length);
	size_t findSet(size_t x);
	void unionSets(size_t x, size_t y);
	~DisjointSets();
};

