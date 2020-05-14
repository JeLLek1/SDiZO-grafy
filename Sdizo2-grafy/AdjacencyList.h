#pragma once

#include "List.h"
#include "Node.h"
#include "AlgoritmReturns.h"

class AdjacencyList
{
private:
	//ilo�� wierzcho�k�w
	size_t nodesLength;
	//ilo�� kraw�dzi
	size_t edgesLength;
	//Tablica list wierzcho�k�w
	List<Node*>** listsArray;

public:
	//czy jest zainicjowana danymi
	bool isActive;
	//Czy ma ujemne wartosci
	bool negativValues;
	//wierzcho�ek startowy dla najkr�tszej �cie�ki i floyda
	size_t nodeStart;
	//wierzcho�ek ko�cowy dla floyda
	size_t nodeEnd;

	AdjacencyList();
	//dodaje kraw�d� do struktury
	void addEdge(size_t nodeFrom, size_t nodeTo, int weight);
	//inicjuje pust� strukture o podanej wielko�ci
	void create(size_t nodesLength, size_t edgesLength);
	//wy�wietlanie struktury
	void display();
	//Ustawienie wierzcho�ka startowego
	void setNodeStart(size_t nodeStart);
	//Ustawienie wierzcho�ka ko�cowego
	void setNodeEnd(size_t nodeEnd);
	//usuwanie wszystkich zaalokowanych dynamicznie element�w i ustawianie warto�ci pocz�tkowych
	void erase();
	//Wyszukiwanie wierzcho�ka ko�cowego
	static Node* find(size_t nodeTo, size_t nodeFrom, List<Node*>** listsArray);
	//Algorytm prima
	SpanningTree* prim();
	//Algorytm Kruskala
	SpanningTree* kruskal();
	//Algorytm Dijkstry
	ShortestPath* dijkstra();
	//Algorytm Bellmana-Forda
	ShortestPath* bellmanFord();
	//Szukanie �cie�ki
	BfsArray* bfs(List<Node*>** listsArray);
	//wyznaczanie maksymalnego przep�ywu
	int fordFulkerson();

	//wszystkie wagi nieujemne
	void makeNonNegative();
	~AdjacencyList();
};

