#pragma once
#include "Node.h"

//Struktura zwracana przez algorytm drzewa rozpinaj�cego
struct SpanningTree {
	//Tablica wska�nik�w na Edge
	Edge** edges;
	//suma wag kraw�dzi drzewa rozpinaj�cego
	int weight;
	//ilo�� kraw�dzi
	size_t edgesLength;
	//inicjalizacja struktruy;
	SpanningTree(size_t length);
	//wy�wietlanie element�w
	void display();
	//Destruktor struktury
	~SpanningTree();
};

//Struktura zwracana przez algorytm najkr�tszej �cie�ki
struct ShortestPath {
	//Tablica wska�nik�w na Nodes
	Node** nodes;
	//wielko�� tablicy
	size_t nodesLength;
	//wierzcho�ek startowy
	size_t nodeStart;
	//czy wartosci ujemne
	bool signedValues;
	//inicjalizacjia struktury
	ShortestPath(size_t nodesLength, size_t nodeStart);
	//Wy�wietlanie struktury
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