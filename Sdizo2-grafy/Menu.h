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
	//Macierz s¹siedztwa
	AdjacencyMatrix* matrix;
	//Lista s¹siedztwa
	AdjacencyList* list;
	//Znak klikniêty na klawiaturze dla obs³ugi menu
	char input;
	//Czy wybrano ju¿ algorytm
	bool algorithmChoosen;
	//Typ algorytmu
	AlgorithmType algorithmType;
	//losowanie wartoœci wed³ug podanej wielkoœci i wype³nienia
	void random(size_t nodesLength, int fillPercent);
	//Wczytywanie z pliku
	bool loadFromFile(std::fstream* file);
	//funkcje pomocnicze wczytywania liczb
	static size_t cinSize_t();
	static int cinuInt(int max);
	//operacja wed³ug wybranej pozycji
	void handleInput();
	//rysowanie menu
	void drawMenu();
	//test szybkoœci struktur
	void test();
public:

	Menu();
	//zwraca iloœæ cyft (i znak - je¿eli liczba ujemna)
	static int numberLength(long number);
	//pêtla g³ówna programu
	void loop();
	~Menu();
};

