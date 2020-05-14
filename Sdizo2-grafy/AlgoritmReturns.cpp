#include <iostream>
#include <string>

#include "AlgoritmReturns.h"
#include "Menu.h"

//
//Drzewo rozpinaj¹ce
//

SpanningTree::SpanningTree(size_t length)
{
	this->edgesLength = length;
	this->weight = 0;
	//tworzenie tablicy krawêdzi
	this->edges = new Edge * [length];
	//tworzenie krawêdzi
	for (size_t i = 0; i < this->edgesLength; i++) {
		this->edges[i] = new Edge;
	}
}

void SpanningTree::display()
{
	std::string output = "";
	output += "Krawedzie drzewa rozpinajacego [ krawedz poczatkowa, krawedz koncowa, waga ]: \n";
	for (size_t i = 0; i < this->edgesLength; i++) {
		output += "[ " + std::to_string(this->edges[i]->nodeFrom) + ", " + std::to_string(this->edges[i]->nodeTo) + ", " + std::to_string(this->edges[i]->weight) + " ]\n";
	}
	output += "\nSuma wag: " + std::to_string(this->weight);
	std::cout << output;
}

SpanningTree::~SpanningTree()
{
	//Usuwanie wszystkich krawêdzi
	for (size_t i = 0; i < this->edgesLength; i++) {
		delete this->edges[i];
	}
	//usuwanie tablicy krawêdzi
	delete this->edges;
}

//
//Najkrótsza œcie¿ka
//


ShortestPath::ShortestPath(size_t nodesLength, size_t nodeStart)
{
	this->nodesLength = nodesLength;
	this->nodeStart = nodeStart;
	this->nodes = new Node * [this->nodesLength];
	//reszta wierzcho³ków
	for (size_t i = 0; i < this->nodesLength; i++) {
		this->nodes[i] = new Node;
		this->nodes[i]->weight = INT_MAX;
		this->nodes[i]->nodeTo = std::numeric_limits<std::size_t>::max();
	}
	//wierzho³ek startowy
	this->nodes[this->nodeStart]->nodeTo = nodeStart;
	this->nodes[this->nodeStart]->weight = 0;
	//czy ujemne wartoœci
	this->signedValues = false;
}

void ShortestPath::display()
{
	std::string output = "";
	output += "Droga dla kazdego z wirzcholkow (numer_wierzcholka [waga_drogi] | droga ): \n";
	int pathLetterCount = 0;
	int nodeLetterCount = Menu::numberLength(static_cast<long>(this->nodesLength) - 1 );
	//Szukanie najd³u¿szego ci¹gu cyfr dla wierzcho³ka i wagi
	for (size_t i = 0; i < this->nodesLength; i++) {
		int tempPathCount = Menu::numberLength(this->nodes[i]->weight);
		if (tempPathCount > pathLetterCount) {
			pathLetterCount = tempPathCount;
		}
	}
	std::cout << output;
	//Wyœwietlanie kolejnych wierzcho³ków
	for (size_t i = 0; i < this->nodesLength; i++) {
		output = "";
		//wyœwietlanie numeru wierzcho³ka

		output += std::to_string(i);
		for (int j = 0; j < nodeLetterCount - Menu::numberLength(static_cast<long>(i)); j++) {
			output += " ";
		}
		//wyœwietlanie wagi
		output += " [ " + std::to_string(this->nodes[i]->weight);
		for (int j = 0; j < pathLetterCount - Menu::numberLength(this->nodes[i]->weight); j++) {
			output += " ";
		}
		//wyœwietlanie kolejnych wierzcho³ków drogi
		output += " ] | " + std::to_string(this->nodes[i]->nodeTo) + " ";
		size_t temp = this->nodes[i]->nodeTo;
		size_t temp1 = this->nodes[temp]->nodeTo;

		while (temp != temp1 && temp1 != std::numeric_limits<std::size_t>::max()) {
			output += "<- " + std::to_string(temp1) + " ";
			temp = temp1;
			temp1 = this->nodes[temp]->nodeTo;
		}
		output += "\n";
		std::cout << output;
	}
}

ShortestPath::~ShortestPath()
{
	//usuwanie wierzcho³ków
	for (size_t i = 0; i < this->nodesLength; i++) {
		delete this->nodes[i];
	}

	//usuwanie tablicy
	delete[] this->nodes;
}

BfsArray::BfsArray(size_t nodesLength)
{
	//tablica sciezek do elementu startowego
	this->nodeArray = new size_t[nodesLength];
	//tablica informacji czy odwiedzono
	this->visited = new bool[nodesLength] ;
	//czy znaleziono œcie¿kê
	this->found = false;

	//inicjalizacja wartoœci pocz¹tkowych
	for (size_t i = 0; i < nodesLength; i++) {
		this->visited[i] = false;
	}
}

BfsArray::~BfsArray()
{
	delete[] this->nodeArray;
	delete[] this->visited;
}
