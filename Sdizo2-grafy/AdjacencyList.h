#pragma once

#include "List.h"
#include "Node.h"
#include "AlgoritmReturns.h"

class AdjacencyList
{
private:
	//iloœæ wierzcho³ków
	size_t nodesLength;
	//iloœæ krawêdzi
	size_t edgesLength;
	//Tablica list wierzcho³ków
	List<Node*>** listsArray;

public:
	//czy jest zainicjowana danymi
	bool isActive;
	//Czy ma ujemne wartosci
	bool negativValues;
	//wierzcho³ek startowy dla najkrótszej œcie¿ki i floyda
	size_t nodeStart;
	//wierzcho³ek koñcowy dla floyda
	size_t nodeEnd;

	AdjacencyList();
	//dodaje krawêdŸ do struktury
	void addEdge(size_t nodeFrom, size_t nodeTo, int weight);
	//inicjuje pust¹ strukture o podanej wielkoœci
	void create(size_t nodesLength, size_t edgesLength);
	//wyœwietlanie struktury
	void display();
	//Ustawienie wierzcho³ka startowego
	void setNodeStart(size_t nodeStart);
	//Ustawienie wierzcho³ka koñcowego
	void setNodeEnd(size_t nodeEnd);
	//usuwanie wszystkich zaalokowanych dynamicznie elementów i ustawianie wartoœci pocz¹tkowych
	void erase();
	//Wyszukiwanie wierzcho³ka koñcowego
	static Node* find(size_t nodeTo, size_t nodeFrom, List<Node*>** listsArray);
	//Algorytm prima
	SpanningTree* prim();
	//Algorytm Kruskala
	SpanningTree* kruskal();
	//Algorytm Dijkstry
	ShortestPath* dijkstra();
	//Algorytm Bellmana-Forda
	ShortestPath* bellmanFord();
	//Szukanie œcie¿ki
	BfsArray* bfs(List<Node*>** listsArray);
	//wyznaczanie maksymalnego przep³ywu
	int fordFulkerson();

	//wszystkie wagi nieujemne
	void makeNonNegative();
	~AdjacencyList();
};

