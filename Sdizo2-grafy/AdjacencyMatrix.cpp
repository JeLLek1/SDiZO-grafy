
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
	//alokowanie pamiêci dla macierzy s¹siadów oraz wype³nianie jej zerami
	this->graph = new int* [nodesLength];
	for (size_t i = 0; i < nodesLength; i++) {
		this->graph[i] = new int[nodesLength];
		//Wype³nianie zerami
		for (size_t j = 0; j < nodesLength; j++) {
			this->graph[i][j] = 0;
		}
	}

	this->isActive = true;
	this->negativValues = true;
}

void AdjacencyMatrix::display()
{
	//Je¿eli graf jest wprowadzony
	if (this->graph != nullptr) {
		std::cout << "macierz sasiedztwa:\n\n";
		int maxLength = Menu::numberLength(static_cast<long>(this->nodesLength)-1);
		//wyszukiwanie, która z wag ma najwiêksz¹ d³ugoœæ
		for (size_t i = 0; i < this->nodesLength; i++) {
			for (size_t j = 0; j < this->nodesLength; j++) {
				int tempLength = Menu::numberLength(this->graph[i][j]);
				if (tempLength > maxLength) {
					maxLength = tempLength;
				}
			}
		}
		//opisy macierzy u góry
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
		//linia oddzielaj¹ca opisy wierzcho³ków od macierzy
		for (size_t i = 0; i < (static_cast<size_t>(maxLength) + 1) * (this->nodesLength + 1) + 1;i++) {
			output += "_";
		}
		output += "\n";
		std::cout << output;
		//Wypisywanie elementów macierzy
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
				//dopisanie wymaganych spacji dla przejrzystoœci
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
	//je¿eli graf jest stworzony
	if (this->graph != nullptr) {
		//usuñ wszyskie tablice na które wskazuj¹c wskaŸniki w tablici this->graph
		for (size_t i = 0; i < this->nodesLength; i++) {
			delete[] this->graph[i];
		}
		//dopiero wtedy mo¿na usun¹æ tablicê wskaŸników
		delete[] this->graph;
	}
	//Przypisanie wartoœci pocz¹tkowych d³ugoœci i wskaŸnikowi
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
	//krawêdzie drzewa rozpinaj¹cego i ich ca³kowita waga w strukturze
	SpanningTree* toReturn = new SpanningTree(this->nodesLength - 1);
	//tablica przechowujaca numer wierzcholka, wierzcho³ek ³¹cz¹cy i wagê krawê¿i
	//Struktura edge ma potrzebne miejsca (w tym wypadku nodeTo - wierzcho³ek do którego jest przypisany element, nodeFrom - wierzcho³ek prowadz¹cy do tego wierzho³ka, waga - waga krawêdzi prowadz¹cej
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
		//wszystkie kolejne mo¿liwe krawêdzie
		for (size_t v = 0; v < this->nodesLength; v++) {
			//jezeli jest krawedz, v nalezy do kolejki i waga krawedzi jest mniejsza niz wczesniejsza
			if (this->graph[nodeFromQueue->nodeTo][v] != 0 && inQueue[v] && this->graph[nodeFromQueue->nodeTo][v] < nodeArray[v]->weight) {
				//nowa waga krawêdzi to ta lepsza
				nodeArray[v]->weight = this->graph[nodeFromQueue->nodeTo][v];
				//wierzcho³ek, z którym jest po³¹czony to ten z kolejki
				nodeArray[v]->nodeFrom = nodeFromQueue->nodeTo;
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
		for (size_t b = a+1; b < this->nodesLength; b++) {
			//je¿eli krawêdŸ istnieje
			if (this->graph[a][b] != 0) {
				//dodaj krawêdŸ do kopca
				Edge* tmp = new Edge();
				tmp->nodeFrom = a;
				tmp->nodeTo = b;
				tmp->weight = this->graph[a][b];
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

ShortestPath* AdjacencyMatrix::dijkstra()
{
	//Struktura przechowuj¹ca informacje o œcie¿kach
	ShortestPath *path = new ShortestPath(this->nodesLength, this->nodeStart);
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
		//szukanie wszystkich s¹siadów wierzcho³ka
		for (size_t i = 0; i < this->nodesLength; i++) {
			//je¿eli waga rozna od 0 (czyli krawêdŸ jest)
			if (this->graph[nodeFromQueue->nodeFrom][i] != 0) {
				//jeœli nowa waga jest lepsza
				if (path->nodes[i]->weight > (this->graph[nodeFromQueue->nodeFrom][i] + nodeFromQueue->weight)) {
					//je¿eli waga ró¿na od nieskoñczonoœci
					if (nodeFromQueue->weight < INT_MAX) {
						//zmiana elementów zwaracej struktury
						path->nodes[i]->weight = this->graph[nodeFromQueue->nodeFrom][i] + nodeFromQueue->weight;
						path->nodes[i]->nodeTo = nodeFromQueue->nodeFrom;
						//zmiana elementów kolejki
						nodeArray[i]->weight = path->nodes[i]->weight;
						nodeArray[i]->nodeTo = path->nodes[i]->nodeTo;
					}
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

ShortestPath* AdjacencyMatrix::bellmanFord()
{
	//Struktura przechowuj¹ca informacje o œcie¿kach
	ShortestPath* path = new ShortestPath(this->nodesLength, this->nodeStart);
	//lista krawêdzi
	List<Edge*> *edgeList = new List<Edge*>();
	for (size_t a = 0; a < this->nodesLength; a++) {
		for (size_t b = 0; b < this->nodesLength; b++) {
			//je¿eli istnieje po³¹czenie
			if (this->graph[a][b] != 0) {
				Edge* tmp = new Edge;
				tmp->nodeFrom = a;
				tmp->nodeTo = b;
				tmp->weight = this->graph[a][b];
				edgeList->pushBack(tmp);
			}
		}
	}
	//relaksacja krawedzi z optymalizacj¹ (je¿eli nie poprawiono drogi to koniec algorytmu)
	bool corrected = true;
	for (size_t i = 0; i < this->nodesLength - 1 && corrected; i++) {
		bool corrected = false;
		//dla ka¿dej krawêdzi
		edgeList->setItHead();
		Edge* edge;
		while ((edge = edgeList->getNext()) != nullptr) {
			//je¿eli lepsza œcie¿ka
			if (path->nodes[edge->nodeTo]->weight > path->nodes[edge->nodeFrom]->weight + this->graph[edge->nodeFrom][edge->nodeTo]) {
				//jest jakaœ poprawa w tej iteracji
				corrected = true;
				//je¿eli waga ró¿na od nieskoñczonoœci
				if (path->nodes[edge->nodeFrom]->weight < INT_MAX) {
					//zmiana wartoœci elementu zwracanego
					path->nodes[edge->nodeTo]->weight = path->nodes[edge->nodeFrom]->weight + this->graph[edge->nodeFrom][edge->nodeTo];
					path->nodes[edge->nodeTo]->nodeTo = edge->nodeFrom;
				}
			}
		}
	}
	//usuwanie listy krawêdzi i sprawdzanie czy nie mia³y wag ujemnych
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
	ListSegment<size_t> *elementIndex;
	while ((elementIndex = queue->getHead()) != nullptr && !returnArray->found) {
		//dla ka¿dej krawêdzi
		for (size_t i = 0; i < this->nodesLength; i++) {
			//je¿eli jest krawêdzi¹ i wierzcho³ek jeszcze nie przetworzony
			if (graph[elementIndex->value][i] > 0 && !returnArray->visited[i]) {
				//odwiedzony, dodany do kolejki 
				returnArray->visited[i] = true;
				queue->pushBack(i);
				//poprzednik
				returnArray->nodeArray[i] = elementIndex->value;
				//je¿eli wierzcho³kiem jest koñcowy wierzcho³ek to znaleziono œcie¿kê
				if (i == this->nodeEnd) {
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

int AdjacencyMatrix::fordFulkerson()
{
	//maksymalny przep³yw
	int returnFlow = 0;
	//kopia grafu
	int **graphCopy = new int* [nodesLength];
	for (size_t i = 0; i < nodesLength; i++) {
		graphCopy[i] = new int[nodesLength];
		//kopiowanie wartoœci
		for (size_t j = 0; j < nodesLength; j++) {
			graphCopy[i][j] = this->graph[i][j];
		}
	}
	//pêtla kolejnych œcie¿ek
	BfsArray* path = this->bfs(graphCopy);

	while (path->found) {

		//szukanie najmniejszej przeustowoœci w œcie¿ce
		//pierwsza krawêdŸ w œcie¿ce
		size_t temp = path->nodeArray[this->nodeEnd];
		int min = graphCopy[temp][this->nodeEnd];
		//przejœcie przez kolejne krawêdzie
		while (temp != this->nodeStart) {
			//je¿eli mniejsza to nowym min jest ta mniejsza wartoœæ
			if (min> graphCopy[path->nodeArray[temp]][temp]) {
				min = graphCopy[path->nodeArray[temp]][temp];
			}
			temp = path->nodeArray[temp];
		}
		//dodanie elementu do wyniku
		returnFlow += min;
		//zmniejszenie przepustowoœci rezydualnej wierzcho³ków w œcie¿ce
		//pierwsza krawêdŸ w œcie¿ce
		temp = path->nodeArray[this->nodeEnd];
		graphCopy[temp][this->nodeEnd] -= min;
		//przejœcie przez kolejne krawêdzie
		while (temp != this->nodeStart) {

			graphCopy[path->nodeArray[temp]][temp]-=min;
			//kana³ w przeciwn¹ stronê
			graphCopy[temp][path->nodeArray[temp]] += min;

			temp = path->nodeArray[temp];
		}
		//ta œcie¿ka jest ju¿ nie potrzebna
		delete path;
		path = this->bfs(graphCopy);
	}

	//usuwanie pêtli œcie¿ek
	delete path;
	//usuñ wszyskie tablice na które wskazuj¹c wskaŸniki w tablici this->graph
	for (size_t i = 0; i < this->nodesLength; i++) {
		delete[] graphCopy[i];
	}
	//dopiero wtedy mo¿na usun¹æ tablicê wskaŸników
	delete[] graphCopy;

	return returnFlow;
}

//Wszystkie krawêdzie nieujemne dla Dijkstry
void AdjacencyMatrix::makeNonNegative()
{
	//je¿eli graf jest stworzony
	if (this->graph != nullptr) {
		this->negativValues = false;
		//Dla wszystkich krawêdzi
		for (size_t a = 0; a < this->nodesLength; a++) {
			for (size_t b = 0; b < this->nodesLength; b++) {
				//jeœli mniejsza od zera
				if (this->graph[a][b] < 0) {
					//wartoœæ przecinwa
					this->graph[a][b] *= -1;
				}

			}
		}
	}
}

AdjacencyMatrix::~AdjacencyMatrix()
{
	//metoda zwalniaj¹ca pamiêæ zajêt¹ przez graf
	this->erase();
}
