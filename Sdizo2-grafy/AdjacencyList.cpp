#include <iostream>
#include <string>
#include <ctime>

#include "AdjacencyList.h"
#include "Menu.h"
#include "Heap.h"
#include "DisjointSets.h"

AdjacencyList::AdjacencyList()
{
	this->nodesLength = 0;
	this->listsArray = nullptr;
	this->nodeStart = 0;
	this->nodeEnd = 0;
	this->isActive = false;
	this->edgesLength = 0;
	this->negativValues = true;
}

void AdjacencyList::addEdge(size_t nodeFrom, size_t nodeTo, int weight)
{
	//tworzenie nowego elementu wierzcho³ek i nawadanie wartoœci
	Node* tmp = new Node();
	tmp->nodeTo = nodeTo;
	tmp->weight = weight;

	//Dodanie do listy wierzcho³ka startowego -> wierzcho³ek koñcowy i wagê
	this->listsArray[nodeFrom]->pushBack(tmp);
}

void AdjacencyList::create(size_t nodesLength, size_t edgesLength)
{
	//Czysczenie starej struktury (je¿eli istnia³¹)
	this->erase();

	this->nodesLength = nodesLength;
	this->edgesLength = edgesLength;
	//alokowanie pamiêci dla tablicy list, oraz alokowanie pamiêci dla List
	this->listsArray = new List<Node*>*[this->nodesLength];
	for (size_t i = 0; i < this->nodesLength; i++) {
		this->listsArray[i] = new List<Node*>();
	}

	this->isActive = true;
	this->negativValues = true;
}

void AdjacencyList::display()
{
	if (this->listsArray != nullptr) {
		std::cout << "Lista sasiedztwa [wierzcholek, waga]:\n\n";
		//iloœc spacji dla oznaczeñ wierzcho³ków startowych
		int maxLength = Menu::numberLength(static_cast<long>(this->nodesLength) - 1);
		std::string output;
		//Dla ka¿dego wierzcho³ka startowego
		for (size_t i = 0; i < this->nodesLength; i++) {
			output = "";
			output += std::to_string(i);
			for (int j = 0; j < (maxLength - Menu::numberLength(static_cast<long>(i))); j++) {
				output += " ";
			}
			output += " | ";
			//Wyœwietlanie kolejnych elementów listy
			this->listsArray[i]->setItHead();
			Node* node;
			while ((node = this->listsArray[i]->getNext()) != nullptr) {
				output+="[";
				output += std::to_string(node->nodeTo);
				output += ", ";
				output += std::to_string(node->weight);
				output += "] ";
			}
			output += "\n";
			std::cout << output;
		}
	}
}

void AdjacencyList::setNodeStart(size_t nodeStart)
{
	if (nodeStart < this->nodesLength) {
		this->nodeStart = nodeStart;
	}
}

void AdjacencyList::setNodeEnd(size_t nodeEnd)
{
	if (nodeEnd < this->nodesLength) {
		this->nodeEnd = nodeEnd;
	}
}

void AdjacencyList::erase()
{
	//jezeli tablica jest zaalokowana
	if (this->listsArray != nullptr) {
		//dla ka¿dej listy
		for (size_t i = 0; i < this->nodesLength; i++) {
			//usuwanie elementów listy
			this->listsArray[i]->setItHead();
			Node* node;
			while ((node = this->listsArray[i]->getNext()) != nullptr) {
				delete node;
			}

			//usuwanie listy
			delete this->listsArray[i];
		}
		//usuwanie tablicy list
		delete[] this->listsArray;
	}
	this->nodesLength = 0;
	this->edgesLength = 0;
	this->listsArray = nullptr;
	this->nodeStart = 0;
	this->nodeEnd = 0;
	this->isActive = false;
	this->negativValues = true;
}

Node* AdjacencyList::find(size_t nodeTo, size_t nodeFrom, List<Node*>** listsArray)
{
	listsArray[nodeFrom]->setItHead();
	Node* node = nullptr;
	//dopuki nie znaleziono elementu idŸ do kolejnego
	while ((node = listsArray[nodeFrom]->getNext()) != nullptr && node->nodeTo != nodeTo) {}
	return node;
}

SpanningTree* AdjacencyList::prim()
{

	//krawêdzie drzewa rozpinaj¹cego i ich ca³kowita waga w strukturze
	SpanningTree* toReturn = new SpanningTree(this->nodesLength - 1);
	//tablica przechowujaca numer wierzcholka, wierzcho³ek ³¹cz¹cy i wagê krawê¿i
	//Struktura edge ma potrzebne miejsca (w tym wypadku nodeTo - wierzcho³ek do którego jest przypisany element, nodeFrom - wierzcho³ek prowadz¹cy do tego wierzho³ka, waga - waga krawêdzi prowadz¹cej
	Edge** nodeArray = new Edge * [this->nodesLength];
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
		inQueue[i] = true;
	}
	//Wierzcho³kiem startowym bêdzie 0
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
		this->listsArray[nodeFromQueue->nodeTo]->setItHead();
		Node* v;
		//kolejne koñce krawêdzi (dopóki s¹ w liœcie)
		while ((v = this->listsArray[nodeFromQueue->nodeTo]->getNext())!=nullptr) {
			//v nalezy do kolejki i waga krawedzi jest mniejsza niz wczesniejsza
			if (inQueue[v->nodeTo] && v->weight < nodeArray[v->nodeTo]->weight) {
				//nowa waga krawêdzi to ta lepsza
				nodeArray[v->nodeTo]->weight = v->weight;
				//wierzcho³ek, z którym jest po³¹czony to ten z kolejki
				nodeArray[v->nodeTo]->nodeFrom = nodeFromQueue->nodeTo;
			}
		}
		//naprawa kolejki priorytetowej po zmianach
		queue->floydConvert();
	}

	//Usuwanie wszystkich wielkoœci pomocniczych i tworzenie listy krawêdzi
	for (size_t i = 0; i < this->nodesLength; i++) {
		//wierzcholek zerowych pomijamy
		if (i != 0) {
			//wierzcho³ek pocz¹tkowy krawêdzi
			toReturn->edges[i - 1]->nodeFrom = nodeArray[i]->nodeFrom;
			//wierzcho³ek koñcowy krawêdzi
			toReturn->edges[i - 1]->nodeTo = nodeArray[i]->nodeTo;
			//waga krawêdzi
			toReturn->edges[i - 1]->weight = nodeArray[i]->weight;
			//waga ca³kowita
			toReturn->weight += nodeArray[i]->weight;
		}
		delete nodeArray[i];
	}
	delete[] nodeArray;
	delete[] inQueue;
	delete queue;

	return toReturn;
}

SpanningTree* AdjacencyList::kruskal()
{
	//krawêdzie drzewa rozpinaj¹cego i ich ca³kowita waga w strukturze
	SpanningTree* toReturn = new SpanningTree(this->nodesLength - 1);
	//iloœæ krawêdzi wprowadzonych
	size_t edgesCount = 0;
	//Zbiory roz³¹czne
	DisjointSets* sets = new DisjointSets(this->nodesLength);
	//Kopiec krawêdzi
	Heap* edgesHeap = new Heap(this->edgesLength);
	//Dodawanie krawêdzi do kopca
	for (size_t a = 0; a < this->nodesLength; a++) {
		//graf nieskierowany, wiêc pomijamy krawêdzie w drug¹ stronê
		this->listsArray[a]->setItHead();
		Node* node;
		while ((node = this->listsArray[a]->getNext()) != nullptr) {
			//jezeli krawêdŸ nie jest dodana do kopca w drug¹ stronê
			if (!edgesHeap->findEdge(node->nodeTo,a)) {
				//dodaj krawêdŸ do kopca
				Edge* tmp = new Edge();
				tmp->nodeFrom = a;
				tmp->nodeTo = node->nodeTo;
				tmp->weight = node->weight;
				edgesHeap->push(tmp);
			}
		}
	}
	//dopuki jest krawêdŸ w kopcu
	Edge* nodeFromQueue;
	while ((nodeFromQueue = edgesHeap->popRoot()) != nullptr) {
		if (sets->findSet(nodeFromQueue->nodeFrom) != sets->findSet(nodeFromQueue->nodeTo)) {
			//Skopiowanie danych krawêdzi do wartoœci zwracanej
			toReturn->edges[edgesCount]->nodeFrom = nodeFromQueue->nodeFrom;
			toReturn->edges[edgesCount]->nodeTo = nodeFromQueue->nodeTo;
			toReturn->edges[edgesCount]->weight = nodeFromQueue->weight;
			//Dodanie wagi ca³kowitej drzewa
			toReturn->weight += nodeFromQueue->weight;

			//po³¹czenie poddrzew w jendo
			sets->unionSets(nodeFromQueue->nodeFrom, nodeFromQueue->nodeTo);
			edgesCount++;
		}
		//krawêdŸ nie bêdzie ju¿ potrzebna
		delete nodeFromQueue;
	}

	//Usuwanie wszystkich wielkoœci pomocniczych i tworzenie listy krawêdzi
	delete sets;
	delete edgesHeap;

	return toReturn;
}

ShortestPath* AdjacencyList::dijkstra()
{
	//Struktura przechowuj¹ca informacje o œcie¿kach
	ShortestPath* path = new ShortestPath(this->nodesLength, this->nodeStart);
	//Kolejka priorytetowa
	Heap* queue = new Heap(this->nodesLength);
	//tablica tych samych elementów aby je zmieniaæ
	Edge** nodeArray = new Edge * [this->nodesLength];
	//Dodanie wszystkich wierzcho³ków z path (tam s¹ ju¿ zainicjowane pocz¹tkowe wartoœci
	for (size_t i = 0; i < this->nodesLength; i++) {
		//Skorzystam z Edge w kolejce, poniewa¿ bêd¹ zajête podobne elementy
		//nodeTo - z którego prowadzi, nodeFrom - obecny element, weight - waga
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
		//dla wszystkich s¹siadów wierzcho³ka
		this->listsArray[nodeFromQueue->nodeFrom]->setItHead();
		Node* node;
		while ((node = this->listsArray[nodeFromQueue->nodeFrom]->getNext()) != nullptr) {
			//jeœli nowa waga jest lepsza
			if (path->nodes[nodeFromQueue->nodeFrom]->weight > (node->weight + nodeFromQueue->weight)) {
				//je¿eli waga ró¿na od nieskoñczonoœci (je¿eli by³yby niepo³¹czone wierzcho³ki)
				if (nodeFromQueue->weight < INT_MAX && nodeFromQueue->nodeFrom< this->nodesLength) {
					//zmiana elementów zwaracej struktury
					path->nodes[nodeFromQueue->nodeFrom]->weight = node->weight + nodeFromQueue->weight;
					path->nodes[nodeFromQueue->nodeFrom]->nodeTo = nodeFromQueue->nodeFrom;
					//zmiana elementów kolejki
					nodeArray[nodeFromQueue->nodeFrom]->weight = path->nodes[nodeFromQueue->nodeFrom]->weight;
					nodeArray[nodeFromQueue->nodeFrom]->nodeTo = path->nodes[nodeFromQueue->nodeFrom]->nodeTo;
				}
			}
		}

		//naprawa kolejki priorytetowej po zmianach
		queue->floydConvert();

		//Nie bêdzie nam potrzebny ju¿ ani w kolejce ani w tablicy elementów z koleik
		delete nodeFromQueue;
	}
	//usuwanie dynamicznie zaalowowanych obiektów pomocniczych
	delete[] nodeArray;
	delete queue;

	return path;
}

ShortestPath* AdjacencyList::bellmanFord()
{
	//Struktura przechowuj¹ca informacje o œcie¿kach
	ShortestPath* path = new ShortestPath(this->nodesLength, this->nodeStart);

	//relaksacja krawedzi z optymalizacj¹ (je¿eli nie poprawiono drogi to koniec algorytmu)
	bool corrected = true;
	for (size_t i = 0; i < this->nodesLength - 1 && corrected; i++) {
		bool corrected = false;
		//dla ka¿dej krawêdzi
		for (size_t j = 0; j < this->nodesLength; j++) {
			this->listsArray[j]->setItHead();
			Node* node;
			while ((node = this->listsArray[j]->getNext()) != nullptr) {
				//je¿eli lepsza œcie¿ka
				if (path->nodes[node->nodeTo]->weight > path->nodes[j]->weight + node->weight) {
					//jest jakaœ poprawa w tej iteracji
					corrected = true;
					//je¿eli waga ró¿na od nieskoñczonoœci (je¿eli by³yby niepo³¹czone wierzcho³ki)
					if (path->nodes[j]->weight < INT_MAX) {
						//zmiana wartoœci elementu zwracanego
						path->nodes[node->nodeTo]->weight = path->nodes[j]->weight + node->weight;
						path->nodes[node->nodeTo]->nodeTo = j;
					}
				}
			}
		}
	}

	//sprawdzanie czy nie ma przebiegu ujemnego
	//dla ka¿dej krawêdzi
	for (size_t j = 0; j < this->nodesLength; j++) {
		this->listsArray[j]->setItHead();
		Node* node;
		while ((node = this->listsArray[j]->getNext()) != nullptr) {
			if (path->nodes[node->nodeTo]->weight > path->nodes[j]->weight+node->weight) {
				path->signedValues = true;
				return path;
			}
		}
	}

	return path;
}

BfsArray* AdjacencyList::bfs(List<Node*>** listsArray)
{
	//tablica œcie¿ki do this->nodeStart do this->nodeEnd, inicjalizacja
	BfsArray* returnArray = new BfsArray(this->nodesLength);
	//kolejka na postawie listy
	List<size_t>* queue = new List<size_t>();

	//inicjalizacja elementu startowego
	returnArray->nodeArray[this->nodeStart] = this->nodeStart;
	returnArray->visited[this->nodeStart] = true;

	//dodanie do kolejki elementu startowego
	queue->pushBack(this->nodeStart);

	//dopuki kolejka nie pusta
	ListSegment<size_t>* elementIndex;
	while ((elementIndex = queue->getHead()) != nullptr && !returnArray->found) {
		//dla ka¿dej krawêdzi
		listsArray[elementIndex->value]->setItHead();
		Node* node;
		while ((node = listsArray[elementIndex->value]->getNext()) != nullptr) {
			//je¿eli krawêdŸ nie zmniejszono do 0 i wierzcho³ek jeszcze nie przetworzony
			if (node->weight > 0 && !returnArray->visited[node->nodeTo]) {
				//odwiedzony, dodany do kolejki 
				returnArray->visited[node->nodeTo] = true;
				queue->pushBack(node->nodeTo);
				//poprzednik
				returnArray->nodeArray[node->nodeTo] = elementIndex->value;
				//je¿eli wierzcho³kiem jest koñcowy wierzcho³ek to znaleziono œcie¿kê
				if (node->nodeTo == this->nodeEnd) {
					returnArray->found = true;
				}
			}
		}
		//usuniêcie g³owy
		queue->popFront();
	}
	//usuwanie kolejki
	delete queue;
	return returnArray;
}

int AdjacencyList::fordFulkerson()
{
	//maksymalny przep³yw
	int returnFlow = 0;
	//kopia grafu
	List<Node*>** coppyListsArray = new List<Node*> * [this->nodesLength];
	for (size_t i = 0; i < this->nodesLength; i++) {
		coppyListsArray[i] = new List<Node*>();
		//kopiowanie wszystkich krawêdzi wierzcho³ka
		//dla ka¿dej krawêdzi
		this->listsArray[i]->setItHead();
		Node* node;
		while ((node = this->listsArray[i]->getNext()) != nullptr) {
			Node* temp = new Node;
			temp->nodeTo = node->nodeTo;
			temp->weight = node->weight;
			coppyListsArray[i]->pushBack(temp);
		}
	}

	//pêtla kolejnych œcie¿ek
	BfsArray* path = this->bfs(coppyListsArray);
	while (path->found) {

		//szukanie najmniejszej przeustowoœci w œcie¿ce
		//pierwsza krawêdŸ w œcie¿ce
		size_t temp = path->nodeArray[this->nodeEnd];
		int min = AdjacencyList::find(this->nodeEnd, temp, coppyListsArray)->weight;
		//przejœcie przez kolejne krawêdzie
		while (temp != this->nodeStart) {
			//je¿eli mniejsza to nowym min jest ta mniejsza wartoœæ
			int weight = AdjacencyList::find(temp, path->nodeArray[temp], coppyListsArray)->weight;
			if (min > weight) {
				min = weight;
			}
			temp = path->nodeArray[temp];
		}
		//dodanie elementu do wyniku
		returnFlow += min;
		//zmniejszenie przepustowoœci rezydualnej wierzcho³ków w œcie¿ce
		//pierwsza krawêdŸ w œcie¿ce
		temp = path->nodeArray[this->nodeEnd];
		AdjacencyList::find(this->nodeEnd, temp, coppyListsArray)->weight -= min;
		//przejœcie przez kolejne krawêdzie
		while (temp != this->nodeStart) {

			AdjacencyList::find(temp, path->nodeArray[temp], coppyListsArray)->weight -= min;
			//w przeciwn¹ stronê
			Node* tempNode = AdjacencyList::find(path->nodeArray[temp], temp, coppyListsArray);
			//je¿eli nie istnieje to dodajemy
			if (tempNode == nullptr) {
				tempNode = new Node;
				tempNode->nodeTo = path->nodeArray[temp];
				tempNode->weight = 0;
				coppyListsArray[temp]->pushBack(tempNode);
			}
			//w drug¹ stronê
			tempNode->weight += min;

			temp = path->nodeArray[temp];
		}
		//ta œcie¿ka jest ju¿ nie potrzebna
		delete path;
		path = this->bfs(coppyListsArray);
	}

	//usuwanie pêtli œcie¿ek
	delete path;
	//usuwanie kopii listy s¹siedztwa
	//dla ka¿dej listy
	for (size_t i = 0; i < this->nodesLength; i++) {
		//usuwanie elementów listy
		coppyListsArray[i]->setItHead();
		Node* node;
		while ((node = coppyListsArray[i]->getNext()) != nullptr) {
			delete node;
		}

		//usuwanie listy
		delete coppyListsArray[i];
	}
	//usuwanie tablicy list
	delete[] coppyListsArray;

	return returnFlow;
}

void AdjacencyList::makeNonNegative()
{
	//jezeli tablica jest zaalokowana
	if (this->listsArray != nullptr) {
		this->negativValues = false;
		std::srand(static_cast<unsigned int>(std::time(NULL)));
		//dla ka¿dej listy
		for (size_t i = 0; i < this->nodesLength; i++) {
			//wartosc bezwzgledna z elementow listy
			this->listsArray[i]->setItHead();
			Node* node;
			while ((node = this->listsArray[i]->getNext()) != nullptr) {
				//jeœli mniejsza od zera
				if (node->weight < 0) {
					//wartoœæ przecinwa
					node->weight *= -1;
				}
			}
		}
	}
}

AdjacencyList::~AdjacencyList()
{
	//metoda zwalniaj¹ca pamiêæ zajêt¹ przez graf
	this->erase();
}
