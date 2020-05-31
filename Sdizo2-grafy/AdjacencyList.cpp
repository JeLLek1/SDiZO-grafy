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
	//tworzenie nowego elementu wierzcho�ek i nawadanie warto�ci
	Node* tmp = new Node();
	tmp->nodeTo = nodeTo;
	tmp->weight = weight;

	//Dodanie do listy wierzcho�ka startowego -> wierzcho�ek ko�cowy i wag�
	this->listsArray[nodeFrom]->pushBack(tmp);
}

void AdjacencyList::create(size_t nodesLength, size_t edgesLength)
{
	//Czysczenie starej struktury (je�eli istnia��)
	this->erase();

	this->nodesLength = nodesLength;
	this->edgesLength = edgesLength;
	//alokowanie pami�ci dla tablicy list, oraz alokowanie pami�ci dla List
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
		//ilo�c spacji dla oznacze� wierzcho�k�w startowych
		int maxLength = Menu::numberLength(static_cast<long>(this->nodesLength) - 1);
		std::string output;
		//Dla ka�dego wierzcho�ka startowego
		for (size_t i = 0; i < this->nodesLength; i++) {
			output = "";
			output += std::to_string(i);
			for (int j = 0; j < (maxLength - Menu::numberLength(static_cast<long>(i))); j++) {
				output += " ";
			}
			output += " | ";
			//Wy�wietlanie kolejnych element�w listy
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
		//dla ka�dej listy
		for (size_t i = 0; i < this->nodesLength; i++) {
			//usuwanie element�w listy
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
	//dopuki nie znaleziono elementu id� do kolejnego
	while ((node = listsArray[nodeFrom]->getNext()) != nullptr && node->nodeTo != nodeTo) {}
	return node;
}

SpanningTree* AdjacencyList::prim()
{

	//kraw�dzie drzewa rozpinaj�cego i ich ca�kowita waga w strukturze
	SpanningTree* toReturn = new SpanningTree(this->nodesLength - 1);
	//tablica przechowujaca numer wierzcholka, wierzcho�ek ��cz�cy i wag� kraw�i
	//Struktura edge ma potrzebne miejsca (w tym wypadku nodeTo - wierzcho�ek do kt�rego jest przypisany element, nodeFrom - wierzcho�ek prowadz�cy do tego wierzho�ka, waga - waga kraw�dzi prowadz�cej
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
		this->listsArray[nodeFromQueue->nodeTo]->setItHead();
		Node* v;
		//kolejne ko�ce kraw�dzi (dop�ki s� w li�cie)
		while ((v = this->listsArray[nodeFromQueue->nodeTo]->getNext())!=nullptr) {
			//v nalezy do kolejki i waga krawedzi jest mniejsza niz wczesniejsza
			if (inQueue[v->nodeTo] && v->weight < nodeArray[v->nodeTo]->weight) {
				//nowa waga kraw�dzi to ta lepsza
				nodeArray[v->nodeTo]->weight = v->weight;
				//wierzcho�ek, z kt�rym jest po��czony to ten z kolejki
				nodeArray[v->nodeTo]->nodeFrom = nodeFromQueue->nodeTo;
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
		this->listsArray[a]->setItHead();
		Node* node;
		while ((node = this->listsArray[a]->getNext()) != nullptr) {
			//jezeli kraw�d� nie jest dodana do kopca w drug� stron�
			if (!edgesHeap->findEdge(node->nodeTo,a)) {
				//dodaj kraw�d� do kopca
				Edge* tmp = new Edge();
				tmp->nodeFrom = a;
				tmp->nodeTo = node->nodeTo;
				tmp->weight = node->weight;
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

ShortestPath* AdjacencyList::dijkstra()
{
	//Struktura przechowuj�ca informacje o �cie�kach
	ShortestPath* path = new ShortestPath(this->nodesLength, this->nodeStart);
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
		//dla wszystkich s�siad�w wierzcho�ka
		this->listsArray[nodeFromQueue->nodeFrom]->setItHead();
		Node* node;
		while ((node = this->listsArray[nodeFromQueue->nodeFrom]->getNext()) != nullptr) {
			//je�li nowa waga jest lepsza
			if (path->nodes[nodeFromQueue->nodeFrom]->weight > (node->weight + nodeFromQueue->weight)) {
				//je�eli waga r�na od niesko�czono�ci (je�eli by�yby niepo��czone wierzcho�ki)
				if (nodeFromQueue->weight < INT_MAX && nodeFromQueue->nodeFrom< this->nodesLength) {
					//zmiana element�w zwaracej struktury
					path->nodes[nodeFromQueue->nodeFrom]->weight = node->weight + nodeFromQueue->weight;
					path->nodes[nodeFromQueue->nodeFrom]->nodeTo = nodeFromQueue->nodeFrom;
					//zmiana element�w kolejki
					nodeArray[nodeFromQueue->nodeFrom]->weight = path->nodes[nodeFromQueue->nodeFrom]->weight;
					nodeArray[nodeFromQueue->nodeFrom]->nodeTo = path->nodes[nodeFromQueue->nodeFrom]->nodeTo;
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

ShortestPath* AdjacencyList::bellmanFord()
{
	//Struktura przechowuj�ca informacje o �cie�kach
	ShortestPath* path = new ShortestPath(this->nodesLength, this->nodeStart);

	//relaksacja krawedzi z optymalizacj� (je�eli nie poprawiono drogi to koniec algorytmu)
	bool corrected = true;
	for (size_t i = 0; i < this->nodesLength - 1 && corrected; i++) {
		bool corrected = false;
		//dla ka�dej kraw�dzi
		for (size_t j = 0; j < this->nodesLength; j++) {
			this->listsArray[j]->setItHead();
			Node* node;
			while ((node = this->listsArray[j]->getNext()) != nullptr) {
				//je�eli lepsza �cie�ka
				if (path->nodes[node->nodeTo]->weight > path->nodes[j]->weight + node->weight) {
					//jest jaka� poprawa w tej iteracji
					corrected = true;
					//je�eli waga r�na od niesko�czono�ci (je�eli by�yby niepo��czone wierzcho�ki)
					if (path->nodes[j]->weight < INT_MAX) {
						//zmiana warto�ci elementu zwracanego
						path->nodes[node->nodeTo]->weight = path->nodes[j]->weight + node->weight;
						path->nodes[node->nodeTo]->nodeTo = j;
					}
				}
			}
		}
	}

	//sprawdzanie czy nie ma przebiegu ujemnego
	//dla ka�dej kraw�dzi
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
	ListSegment<size_t>* elementIndex;
	while ((elementIndex = queue->getHead()) != nullptr && !returnArray->found) {
		//dla ka�dej kraw�dzi
		listsArray[elementIndex->value]->setItHead();
		Node* node;
		while ((node = listsArray[elementIndex->value]->getNext()) != nullptr) {
			//je�eli kraw�d� nie zmniejszono do 0 i wierzcho�ek jeszcze nie przetworzony
			if (node->weight > 0 && !returnArray->visited[node->nodeTo]) {
				//odwiedzony, dodany do kolejki 
				returnArray->visited[node->nodeTo] = true;
				queue->pushBack(node->nodeTo);
				//poprzednik
				returnArray->nodeArray[node->nodeTo] = elementIndex->value;
				//je�eli wierzcho�kiem jest ko�cowy wierzcho�ek to znaleziono �cie�k�
				if (node->nodeTo == this->nodeEnd) {
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

int AdjacencyList::fordFulkerson()
{
	//maksymalny przep�yw
	int returnFlow = 0;
	//kopia grafu
	List<Node*>** coppyListsArray = new List<Node*> * [this->nodesLength];
	for (size_t i = 0; i < this->nodesLength; i++) {
		coppyListsArray[i] = new List<Node*>();
		//kopiowanie wszystkich kraw�dzi wierzcho�ka
		//dla ka�dej kraw�dzi
		this->listsArray[i]->setItHead();
		Node* node;
		while ((node = this->listsArray[i]->getNext()) != nullptr) {
			Node* temp = new Node;
			temp->nodeTo = node->nodeTo;
			temp->weight = node->weight;
			coppyListsArray[i]->pushBack(temp);
		}
	}

	//p�tla kolejnych �cie�ek
	BfsArray* path = this->bfs(coppyListsArray);
	while (path->found) {

		//szukanie najmniejszej przeustowo�ci w �cie�ce
		//pierwsza kraw�d� w �cie�ce
		size_t temp = path->nodeArray[this->nodeEnd];
		int min = AdjacencyList::find(this->nodeEnd, temp, coppyListsArray)->weight;
		//przej�cie przez kolejne kraw�dzie
		while (temp != this->nodeStart) {
			//je�eli mniejsza to nowym min jest ta mniejsza warto��
			int weight = AdjacencyList::find(temp, path->nodeArray[temp], coppyListsArray)->weight;
			if (min > weight) {
				min = weight;
			}
			temp = path->nodeArray[temp];
		}
		//dodanie elementu do wyniku
		returnFlow += min;
		//zmniejszenie przepustowo�ci rezydualnej wierzcho�k�w w �cie�ce
		//pierwsza kraw�d� w �cie�ce
		temp = path->nodeArray[this->nodeEnd];
		AdjacencyList::find(this->nodeEnd, temp, coppyListsArray)->weight -= min;
		//przej�cie przez kolejne kraw�dzie
		while (temp != this->nodeStart) {

			AdjacencyList::find(temp, path->nodeArray[temp], coppyListsArray)->weight -= min;
			//w przeciwn� stron�
			Node* tempNode = AdjacencyList::find(path->nodeArray[temp], temp, coppyListsArray);
			//je�eli nie istnieje to dodajemy
			if (tempNode == nullptr) {
				tempNode = new Node;
				tempNode->nodeTo = path->nodeArray[temp];
				tempNode->weight = 0;
				coppyListsArray[temp]->pushBack(tempNode);
			}
			//w drug� stron�
			tempNode->weight += min;

			temp = path->nodeArray[temp];
		}
		//ta �cie�ka jest ju� nie potrzebna
		delete path;
		path = this->bfs(coppyListsArray);
	}

	//usuwanie p�tli �cie�ek
	delete path;
	//usuwanie kopii listy s�siedztwa
	//dla ka�dej listy
	for (size_t i = 0; i < this->nodesLength; i++) {
		//usuwanie element�w listy
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
		//dla ka�dej listy
		for (size_t i = 0; i < this->nodesLength; i++) {
			//wartosc bezwzgledna z elementow listy
			this->listsArray[i]->setItHead();
			Node* node;
			while ((node = this->listsArray[i]->getNext()) != nullptr) {
				//je�li mniejsza od zera
				if (node->weight < 0) {
					//warto�� przecinwa
					node->weight *= -1;
				}
			}
		}
	}
}

AdjacencyList::~AdjacencyList()
{
	//metoda zwalniaj�ca pami�� zaj�t� przez graf
	this->erase();
}
