#pragma once
#include "Node.h"

//Struktura zwracana przez algorytm drzewa rozpinaj¹cego
struct SpanningTree {
	//Tablica wskaŸników na Edge
	Edge** edges;
	//suma wag krawêdzi drzewa rozpinaj¹cego
	int weight;
	//iloœæ krawêdzi
	size_t edgesLength;
	//inicjalizacja struktruy;
	SpanningTree(size_t length);
	//wyœwietlanie elementów
	void display();
	//Destruktor struktury
	~SpanningTree();
};

//Struktura zwracana przez algorytm najkrótszej œcie¿ki
struct ShortestPath {
	//Tablica wskaŸników na Nodes
	Node** nodes;
	//wielkoœæ tablicy
	size_t nodesLength;
	//wierzcho³ek startowy
	size_t nodeStart;
	//czy wartosci ujemne
	bool signedValues;
	//inicjalizacjia struktury
	ShortestPath(size_t nodesLength, size_t nodeStart);
	//Wyœwietlanie struktury
	void display();
	//Destruktor struktury
	~ShortestPath();
};

//Struktura wykorzystywana przez algorytm bfs
struct BfsArray {
	size_t* nodeArray;
	bool* visited;
	bool found;
	
	//inicjalizacja
	BfsArray(size_t nodesLength);
	//destruktor
	~BfsArray();
};