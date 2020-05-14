
#include <iostream>
#include <string>
#include <ctime>

#include "AdjacencyMatrix.h"
#include "Menu.h"
#include "Heap.h"
#include "DisjointSets.h"
#include "List.h"

AdjacencyMatrix::AdjacencyMatrix()
{
	this->nodesLength = 0;
	this->edgesLength = 0;
	this->graph = nullptr;
	this->nodeStart = 0;
	this->nodeEnd = 0;
	this->isActive = false;
	this->negativValues = true;
}

void AdjacencyMatrix::addEdge(size_t nodeFrom, size_t nodeTo, int weight)
{
	this->graph[nodeFrom][nodeTo] = weight;
	this->edgesLength++;
}

void AdjacencyMatrix::create(size_t nodesLength, size_t edgesLength)
{
	//Czyszczenie starej macierzy
	this->erase();

	this->nodesLength = nodesLength;
	this->edgesLength = edgesLength;
	//alokowanie pami�ci dla macierzy s�siad�w oraz wype�nianie jej zerami
	this->graph = new int* [nodesLength];
	for (size_t i = 0; i < nodesLength; i++) {
		this->graph[i] = new int[nodesLength];
		//Wype�nianie zerami
		for (size_t j = 0; j < nodesLength; j++) {
			this->graph[i][j] = 0;
		}
	}

	this->isActive = true;
	this->negativValues = true;
}

void AdjacencyMatrix::display()
{
	//Je�eli graf jest wprowadzony
	if (this->graph != nullptr) {
		std::cout << "macierz sasiedztwa:\n\n";
		int maxLength = Menu::numberLength(static_cast<long>(this->nodesLength)-1);
		//wyszukiwanie, kt�ra z wag ma najwi�ksz� d�ugo��
		for (size_t i = 0; i < this->nodesLength; i++) {
			for (size_t j = 0; j < this->nodesLength; j++) {
				int tempLength = Menu::numberLength(this->graph[i][j]);
				if (tempLength > maxLength) {
					maxLength = tempLength;
				}
			}
		}
		//opisy macierzy u g�ry
		std::string output = "";
		for (int i = 0; i < maxLength; i++) {
			output += " ";
		}
		output += "| ";
		for (size_t i = 0; i < this->nodesLength; i++) {
			output += std::to_string(i);
			for (int j = 0; j < (maxLength - Menu::numberLength(static_cast<long>(i))); j++) {
				output += " ";
			}
			output += " ";
		}
		output += "|\n";
		//linia oddzielaj�ca opisy wierzcho�k�w od macierzy
		for (size_t i = 0; i < (static_cast<size_t>(maxLength) + 1) * (this->nodesLength + 1) + 1;i++) {
			output += "_";
		}
		output += "\n";
		std::cout << output;
		//Wypisywanie element�w macierzy
		for (size_t i = 0; i < this->nodesLength; i++) {
			output = "";
			output += std::to_string(i);
			for (int j = 0; j < (maxLength - Menu::numberLength(static_cast<long>(i))); j++) {
				output += " ";
			}
			output += "| ";
			for (size_t j = 0; j < this->nodesLength; j++) {
				output += std::to_string(this->graph[i][j]);
				output += " ";
				//dopisanie wymaganych spacji dla przejrzysto�ci
				for (int k = 0; k < (maxLength - Menu::numberLength(this->graph[i][j])); k++) {
					output += " ";
				}
			}
			output += "|\n";
			std::cout << output;
		}
	}
}

void AdjacencyMatrix::setNodeStart(size_t nodeStart)
{
	if (nodeStart < this->nodesLength) {
		this->nodeStart = nodeStart;
	}
}

void AdjacencyMatrix::setNodeEnd(size_t nodeEnd)
{
	if (nodeEnd < this->nodesLength) {
		this->nodeEnd = nodeEnd;
	}
}

void AdjacencyMatrix::erase()
{
	//je�eli graf jest stworzony
	if (this->graph != nullptr) {
		//usu� wszyskie tablice na kt�re wskazuj�c wska�niki w tablici this->graph
		for (size_t i = 0; i < this->nodesLength; i++) {
			delete[] this->graph[i];
		}
		//dopiero wtedy mo�na usun�� tablic� wska�nik�w
		delete[] this->graph;
	}
	//Przypisanie warto�ci pocz�tkowych d�ugo�ci i wska�nikowi
	this->nodesLength = 0;
	this->edgesLength = 0;
	this->graph = nullptr;
	this->nodeStart = 0;
	this->nodeEnd = 0;
	this->isActive = false;
	this->negativValues = true;
}

SpanningTree* AdjacencyMatrix::prim()
{
	//kraw�dzie drzewa rozpinaj�cego i ich ca�kowita waga w strukturze
	SpanningTree* toReturn = new SpanningTree(this->nodesLength - 1);
	//tablica przechowujaca numer wierzcholka, wierzcho�ek ��cz�cy i wag� kraw�i
	//Struktura edge ma potrzebne miejsca (w tym wypadku nodeTo - wierzcho�ek do kt�rego jest przypisany element, nodeFrom - wierzcho�ek prowadz�cy do tego wierzho�ka, waga - waga kraw�dzi prowadz�cej
	Edge** nodeArray = new Edge *[this->nodesLength];
	//Kolejka priorytetowa
	Heap* queue = new Heap(this->nodesLength);
	//Czy element jest w kolejce
	bool* inQueue = new bool[this->nodesLength];
	//wartosci poczatkowe dla zmiennych pomocniczych
	for (size_t i = 1; i < this->nodesLength; i++) {
		nodeArray[i] = new Edge();
		
		nodeArray[i]->nodeTo = i;
		nodeArray[i]->weight = INT_MAX;
		//Kolejka priorytetowa
		queue->push(nodeArray[i]);
	}
	//Wierzcho�kiem startowym b�dzie 0
	nodeArray[0] = new Edge();
	nodeArray[0]->nodeTo = 0;
	nodeArray[0]->nodeFrom = 0;
	nodeArray[0]->weight = 0;
	queue->push(nodeArray[0]);
	inQueue[0] = true;

	//petla wedlug kolejki priorytetowej
	Edge* nodeFromQueue;
	while ((nodeFromQueue = queue->popRoot()) != nullptr) {
		inQueue[nodeFromQueue->nodeTo] = false;
		//wszystkie kolejne mo�liwe kraw�dzie
		for (size_t v = 0; v < this->nodesLength; v++) {
			//jezeli jest krawedz, v nalezy do kolejki i waga krawedzi jest mniejsza niz wczesniejsza
			if (this->graph[nodeFromQueue->nodeTo][v] != 0 && inQueue[v] && this->graph[nodeFromQueue->nodeTo][v] < nodeArray[v]->weight) {
				//nowa waga kraw�dzi to ta lepsza
				nodeArray[v]->weight = this->graph[nodeFromQueue->nodeTo][v];
				//wierzcho�ek, z kt�rym jest po��czony to ten z kolejki
				nodeArray[v]->nodeFrom = nodeFromQueue->nodeTo;
			}
		}
		//naprawa kolejki priorytetowej po zmianach
		queue->floydConvert();
	}

	//Usuwanie wszystkich wielko�ci pomocniczych i tworzenie listy kraw�dzi
	for (size_t i = 0; i < this->nodesLength; i++) {
		//wierzcholek zerowych pomijamy
		if (i != 0) {
			//wierzcho�ek pocz�tkowy kraw�dzi
			toReturn->edges[i - 1]->nodeFrom = nodeArray[i]->nodeFrom;
			//wierzcho�ek ko�cowy kraw�dzi
			toReturn->edges[i - 1]->nodeTo = nodeArray[i]->nodeTo;
			//waga kraw�dzi
			toReturn->edges[i - 1]->weight = nodeArray[i]->weight;
			//waga ca�kowita
			toReturn->weight+= nodeArray[i]->weight;
		}
		delete nodeArray[i];
	}
	delete[] nodeArray;
	delete[] inQueue;
	delete queue;

	return toReturn;
}

SpanningTree* AdjacencyMatrix::kruskal()
{
	//kraw�dzie drzewa rozpinaj�cego i ich ca�kowita waga w strukturze
	SpanningTree* toReturn = new SpanningTree(this->nodesLength - 1);
	//ilo�� kraw�dzi wprowadzonych
	size_t edgesCount = 0;
	//Zbiory roz��czne
	DisjointSets* sets = new DisjointSets(this->nodesLength);
	//Kopiec kraw�dzi
	Heap* edgesHeap = new Heap(this->edgesLength);
	//Dodawanie kraw�dzi do kopca
	for (size_t a = 0; a < this->nodesLength; a++) {
		//graf nieskierowany, wi�c pomijamy kraw�dzie w drug� stron�
		for (size_t b = a+1; b < this->nodesLength; b++) {
			//je�eli kraw�d� istnieje
			if (this->graph[a][b] != 0) {
				//dodaj kraw�d� do kopca
				Edge* tmp = new Edge();
				tmp->nodeFrom = a;
				tmp->nodeTo = b;
				tmp->weight = this->graph[a][b];
				edgesHeap->push(tmp);
			}
		}
	}
	//dopuki jest kraw�d� w kopcu
	Edge* nodeFromQueue;
	while ((nodeFromQueue = edgesHeap->popRoot()) != nullptr) {
		if (sets->findSet(nodeFromQueue->nodeFrom) != sets->findSet(nodeFromQueue->nodeTo)) {
			//Skopiowanie danych kraw�dzi do warto�ci zwracanej
			toReturn->edges[edgesCount]->nodeFrom = nodeFromQueue->nodeFrom;
			toReturn->edges[edgesCount]->nodeTo = nodeFromQueue->nodeTo;
			toReturn->edges[edgesCount]->weight = nodeFromQueue->weight;
			//Dodanie wagi ca�kowitej drzewa
			toReturn->weight += nodeFromQueue->weight;

			//po��czenie poddrzew w jendo
			sets->unionSets(nodeFromQueue->nodeFrom, nodeFromQueue->nodeTo);
			edgesCount++;
		}
		//kraw�d� nie b�dzie ju� potrzebna
		delete nodeFromQueue;
	}

	//Usuwanie wszystkich wielko�ci pomocniczych i tworzenie listy kraw�dzi
	delete sets;
	delete edgesHeap;

	return toReturn;
}

ShortestPath* AdjacencyMatrix::dijkstra()
{
	//Struktura przechowuj�ca informacje o �cie�kach
	ShortestPath *path = new ShortestPath(this->nodesLength, this->nodeStart);
	//Kolejka priorytetowa
	Heap* queue = new Heap(this->nodesLength);
	//tablica tych samych element�w aby je zmienia�
	Edge** nodeArray = new Edge * [this->nodesLength];
	//Dodanie wszystkich wierzcho�k�w z path (tam s� ju� zainicjowane pocz�tkowe warto�ci
	for (size_t i = 0; i < this->nodesLength; i++) {
		//Skorzystam z Edge w kolejce, poniewa� b�d� zaj�te podobne elementy
		//nodeTo - z kt�rego prowadzi, nodeFrom - obecny element, weight - waga
		Edge* tmp = new Edge;
		tmp->weight = path->nodes[i]->weight;
		tmp->nodeTo = path->nodes[i]->nodeTo;
		tmp->nodeFrom = i;
		queue->push(tmp);
		nodeArray[i] = tmp;
	}

	//petla wedlug kolejki priorytetowej
	Edge* nodeFromQueue;
	while ((nodeFromQueue = queue->popRoot()) != nullptr) {
		//szukanie wszystkich s�siad�w wierzcho�ka
		for (size_t i = 0; i < this->nodesLength; i++) {
			//je�eli waga rozna od 0 (czyli kraw�d� jest)
			if (this->graph[nodeFromQueue->nodeFrom][i] != 0) {
				//je�li nowa waga jest lepsza
				if (path->nodes[i]->weight > (this->graph[nodeFromQueue->nodeFrom][i] + nodeFromQueue->weight)) {
					//je�eli waga r�na od niesko�czono�ci
					if (nodeFromQueue->weight < INT_MAX) {
						//zmiana element�w zwaracej struktury
						path->nodes[i]->weight = this->graph[nodeFromQueue->nodeFrom][i] + nodeFromQueue->weight;
						path->nodes[i]->nodeTo = nodeFromQueue->nodeFrom;
						//zmiana element�w kolejki
						nodeArray[i]->weight = path->nodes[i]->weight;
						nodeArray[i]->nodeTo = path->nodes[i]->nodeTo;
					}
				}
			}
		}
		//naprawa kolejki priorytetowej po zmianach
		queue->floydConvert();

		//Nie b�dzie nam potrzebny ju� ani w kolejce ani w tablicy element�w z koleik
		delete nodeFromQueue;
	}
	//usuwanie dynamicznie zaalowowanych obiekt�w pomocniczych
	delete[] nodeArray;
	delete queue;

	return path;
}

ShortestPath* AdjacencyMatrix::bellmanFord()
{
	//Struktura przechowuj�ca informacje o �cie�kach
	ShortestPath* path = new ShortestPath(this->nodesLength, this->nodeStart);
	//lista kraw�dzi
	List<Edge*> *edgeList = new List<Edge*>();
	for (size_t a = 0; a < this->nodesLength; a++) {
		for (size_t b = 0; b < this->nodesLength; b++) {
			//je�eli istnieje po��czenie
			if (this->graph[a][b] != 0) {
				Edge* tmp = new Edge;
				tmp->nodeFrom = a;
				tmp->nodeTo = b;
				tmp->weight = this->graph[a][b];
				edgeList->pushBack(tmp);
			}
		}
	}
	//relaksacja krawedzi z optymalizacj� (je�eli nie poprawiono drogi to koniec algorytmu)
	bool corrected = true;
	for (size_t i = 0; i < this->nodesLength - 1 && corrected; i++) {
		bool corrected = false;
		//dla ka�dej kraw�dzi
		edgeList->setItHead();
		Edge* edge;
		while ((edge = edgeList->getNext()) != nullptr) {
			//je�eli lepsza �cie�ka
			if (path->nodes[edge->nodeTo]->weight > path->nodes[edge->nodeFrom]->weight + this->graph[edge->nodeFrom][edge->nodeTo]) {
				//jest jaka� poprawa w tej iteracji
				corrected = true;
				//je�eli waga r�na od niesko�czono�ci
				if (path->nodes[edge->nodeFrom]->weight < INT_MAX) {
					//zmiana warto�ci elementu zwracanego
					path->nodes[edge->nodeTo]->weight = path->nodes[edge->nodeFrom]->weight + this->graph[edge->nodeFrom][edge->nodeTo];
					path->nodes[edge->nodeTo]->nodeTo = edge->nodeFrom;
				}
			}
		}
	}
	//usuwanie listy kraw�dzi i sprawdzanie czy nie mia�y wag ujemnych
	edgeList->setItHead();
	Edge* edge;
	while ((edge = edgeList->getNext()) != nullptr) {
		if (edge->weight < 0) {
			path->signedValues = true;
		}
		delete edge;
	}
	delete edgeList;

	return path;
}

BfsArray* AdjacencyMatrix::bfs(int** graph)
{
	//tablica �cie�ki do this->nodeStart do this->nodeEnd, inicjalizacja
	BfsArray* returnArray = new BfsArray(this->nodesLength);
	//kolejka na postawie listy
	List<size_t>* queue = new List<size_t>();
	
	//inicjalizacja elementu startowego
	returnArray->nodeArray[this->nodeStart] = this->nodeStart;
	returnArray->visited[this->nodeStart] = true;

	//dodanie do kolejki elementu startowego
	queue->pushBack(this->nodeStart);

	//dopuki kolejka nie pusta
	ListSegment<size_t> *elementIndex;
	while ((elementIndex = queue->getHead()) != nullptr && !returnArray->found) {
		//dla ka�dej kraw�dzi
		for (size_t i = 0; i < this->nodesLength; i++) {
			//je�eli jest kraw�dzi� i wierzcho�ek jeszcze nie przetworzony
			if (graph[elementIndex->value][i] > 0 && !returnArray->visited[i]) {
				//odwiedzony, dodany do kolejki 
				returnArray->visited[i] = true;
				queue->pushBack(i);
				//poprzednik
				returnArray->nodeArray[i] = elementIndex->value;
				//je�eli wierzcho�kiem jest ko�cowy wierzcho�ek to znaleziono �cie�k�
				if (i == this->nodeEnd) {
					returnArray->found = true;
				}
			}
		}
		//usuni�cie g�owy
		queue->popFront();
	}
	//usuwanie kolejki
	delete queue;
	return returnArray;
}

int AdjacencyMatrix::fordFulkerson()
{
	//maksymalny przep�yw
	int returnFlow = 0;
	//kopia grafu
	int **graphCopy = new int* [nodesLength];
	for (size_t i = 0; i < nodesLength; i++) {
		graphCopy[i] = new int[nodesLength];
		//kopiowanie warto�ci
		for (size_t j = 0; j < nodesLength; j++) {
			graphCopy[i][j] = this->graph[i][j];
		}
	}
	//p�tla kolejnych �cie�ek
	BfsArray* path = this->bfs(graphCopy);

	while (path->found) {

		//szukanie najmniejszej przeustowo�ci w �cie�ce
		//pierwsza kraw�d� w �cie�ce
		size_t temp = path->nodeArray[this->nodeEnd];
		int min = graphCopy[temp][this->nodeEnd];
		//przej�cie przez kolejne kraw�dzie
		while (temp != this->nodeStart) {
			//je�eli mniejsza to nowym min jest ta mniejsza warto��
			if (min> graphCopy[path->nodeArray[temp]][temp]) {
				min = graphCopy[path->nodeArray[temp]][temp];
			}
			temp = path->nodeArray[temp];
		}
		//dodanie elementu do wyniku
		returnFlow += min;
		//zmniejszenie przepustowo�ci rezydualnej wierzcho�k�w w �cie�ce
		//pierwsza kraw�d� w �cie�ce
		temp = path->nodeArray[this->nodeEnd];
		graphCopy[temp][this->nodeEnd] -= min;
		//przej�cie przez kolejne kraw�dzie
		while (temp != this->nodeStart) {

			graphCopy[path->nodeArray[temp]][temp]-=min;
			//kana� w przeciwn� stron�
			graphCopy[temp][path->nodeArray[temp]] += min;

			temp = path->nodeArray[temp];
		}
		//ta �cie�ka jest ju� nie potrzebna
		delete path;
		path = this->bfs(graphCopy);
	}

	//usuwanie p�tli �cie�ek
	delete path;
	//usu� wszyskie tablice na kt�re wskazuj�c wska�niki w tablici this->graph
	for (size_t i = 0; i < this->nodesLength; i++) {
		delete[] graphCopy[i];
	}
	//dopiero wtedy mo�na usun�� tablic� wska�nik�w
	delete[] graphCopy;

	return returnFlow;
}

//Wszystkie kraw�dzie nieujemne dla Dijkstry
void AdjacencyMatrix::makeNonNegative()
{
	//je�eli graf jest stworzony
	if (this->graph != nullptr) {
		this->negativValues = false;
		//Dla wszystkich kraw�dzi
		for (size_t a = 0; a < this->nodesLength; a++) {
			for (size_t b = 0; b < this->nodesLength; b++) {
				//je�li mniejsza od zera
				if (this->graph[a][b] < 0) {
					//warto�� przecinwa
					this->graph[a][b] *= -1;
				}

			}
		}
	}
}

AdjacencyMatrix::~AdjacencyMatrix()
{
	//metoda zwalniaj�ca pami�� zaj�t� przez graf
	this->erase();
}
