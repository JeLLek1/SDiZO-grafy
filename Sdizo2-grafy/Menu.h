#pragma once
#include <fstream>

#include "AdjacencyMatrix.h"
#include "AdjacencyList.h"
#include "AlgorithmType.h"

class Menu
{
private:
	//opcja w menu
	int option;
	//Macierz s�siedztwa
	AdjacencyMatrix* matrix;
	//Lista s�siedztwa
	AdjacencyList* list;
	//Znak klikni�ty na klawiaturze dla obs�ugi menu
	char input;
	//Czy wybrano ju� algorytm
	bool algorithmChoosen;
	//Typ algorytmu
	AlgorithmType algorithmType;
	//losowanie warto�ci wed�ug podanej wielko�ci i wype�nienia
	void random(size_t nodesLength, int fillPercent);
	//Wczytywanie z pliku
	bool loadFromFile(std::fstream* file);
	//funkcje pomocnicze wczytywania liczb
	static size_t cinSize_t();
	static int cinuInt(int max);
	//operacja wed�ug wybranej pozycji
	void handleInput();
	//rysowanie menu
	void drawMenu();
	//test szybko�ci struktur
	void test();
public:

	Menu();
	//zwraca ilo�� cyft (i znak - je�eli liczba ujemna)
	static int numberLength(long number);
	//p�tla g��wna programu
	void loop();
	~Menu();
};

