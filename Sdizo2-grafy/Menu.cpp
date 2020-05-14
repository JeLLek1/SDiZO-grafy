#include <ctime>
#include <iostream>
#include <string>
#include <conio.h>

#include "Menu.h"
#include "Config.h"
#include "List.h"
#include "TimeMeasure.h"

void Menu::random(size_t nodesLength, int fillPercent)
{
	//ile krawêdzi trzeba wygenerowaæ (n*(n-1))/2 * (fillpercent/100)
	size_t toGenerate = ((nodesLength* (nodesLength - 1)) * fillPercent) / 100;

	//jeœli graf nieskierowany to 2 razy mniej
	if (this->algorithmType == AlgorithmType::MST) {
		toGenerate >>= 1;
	}
	//metoda alokujaca i inicjujaca grafy
	matrix->create(nodesLength, toGenerate);
	list->create(nodesLength, toGenerate);

	//Lista wszystkich mo¿liwych krawêdzi
	List<size_t*> *poslibleEdges = new List<size_t*>();
	for (size_t i = 0; i < nodesLength; i++) {
		for (size_t j = i+1; j < nodesLength; j++) {
			size_t *array = new size_t[2];
			//node from
			array[0] = i;
			//node to
			array[1] = j;

			poslibleEdges->pushBack(array);
			//Je¿eli krawêdzie jednokierunkowe to równie¿ krawêdŸ w druga stronê
			if (this->algorithmType != AlgorithmType::MST) {
				array = new size_t[2];
				//node from
				array[0] = j;
				//node to
				array[1] = i;

				poslibleEdges->pushBack(array);
			}
		}
	}


	std::srand(static_cast<unsigned int>(std::time(NULL)));
	//Tworzenie losowego drzewa rozpinajacego
	for (size_t i = 1; i < nodesLength; i++) {
		//losowanie wêz³a startowego (mo¿na tylko po³aczyæ do tych, które ju¿ maja po³aczenie)
		size_t nodeFrom = static_cast<size_t>(std::rand() % i);
		//dla MST bêda losowane te¿ wartoœci ujemne, dla reszty nie ma takiej potrzeby bo wyniki bêda nieadekwatne
		int randomWeight;
		if (this->algorithmType == AlgorithmType::MST) {
			//losowanie wagi <MAX_EDGE_WEIGHT;MAX_EDGE_WEIGHT> / {0}
			randomWeight = ((std::rand() % MAX_EDGE_WEIGHT) + 1);
			if (std::rand() % 2 == 1) {
				randomWeight *= -1;
			}
		}
		else {
			//losowanie wagi <1;MAX_EDGE_WEIGHT>
			randomWeight = (std::rand() % MAX_EDGE_WEIGHT)+1;
		}
		//przypisanie wagi do wylosowanej krawêdzi
		matrix->addEdge(nodeFrom, i, randomWeight);
		list->addEdge(nodeFrom, i, randomWeight);
		//je¿eli jest to graf nieskierowany (algorytm MST)
		if (this->algorithmType == AlgorithmType::MST) {
			//to macierz symetryczna wzglêdem g³ównej przekatnej
			matrix->addEdge(i, nodeFrom, randomWeight);
			list->addEdge(i, nodeFrom, randomWeight);
		}

		//Usuwanie z listy elementów ju¿ wygenerowanych
		poslibleEdges->setItHead();
		size_t index = 0;
		size_t* element;
		if (this->algorithmType == AlgorithmType::MST) {
			//dopuki istnieje kolejny element i nie zosta³ znaleziony ten wstawiony
			while ((element = poslibleEdges->getNext())!=nullptr &&
					((element[0] != i || element[1] != nodeFrom) &&
						(element[0] != nodeFrom || element[1] != i))) {
				index++;
			}
		}
		else {
			//dopuki istnieje kolejny element i nie zosta³ znaleziony wstawiony
			while ((element = poslibleEdges->getNext()) != nullptr &&
				(element[0] != nodeFrom || element[1] != i)) {
				index++;
			}
		}
		poslibleEdges->pop(index);
	}

	//Liczba krawêdzi pozosta³ych do wygenerowania
	//(uwaga, nie da siê uniknaæ wype³nienia minimalnego [nodesLength-1]/maxEdges z powodu drzewa rozpinajacego)
	if (fillPercent <= 100 && toGenerate > nodesLength - 1) {
		//iloœæ krawêdzi pozosta³ych do wygenerowania
		toGenerate = toGenerate  - nodesLength + 1;
		//dopuki wype³nienie nie jest równe zadanemu
		while (toGenerate > 0) {
			//Wybieranie losowej krawêdzi
			size_t randomEdge = std::rand() % poslibleEdges->size;
			//Pobranie krawêdzi o lowym indeksie
			size_t* edge = poslibleEdges->searchEl(randomEdge);
			//dla MST bêda losowane te¿ wartoœci ujemne, dla reszty nie ma takiej potrzeby bo wyniki bêda nieadekwatne
			int randomWeight;
			if (this->algorithmType == AlgorithmType::MST) {
				//losowanie wagi <MAX_EDGE_WEIGHT;MAX_EDGE_WEIGHT> / {0}
				randomWeight = ((std::rand() % MAX_EDGE_WEIGHT) + 1);
				if (std::rand() % 2 == 1) {
					randomWeight *= -1;
				}
			}
			else {
				//losowanie wagi <1;MAX_EDGE_WEIGHT>
				randomWeight = (std::rand() % MAX_EDGE_WEIGHT)+1;
			}
			//przypisanie wagi do wylosowanej krawêdzi
			matrix->addEdge(edge[0], edge[1], randomWeight);
			list->addEdge(edge[0], edge[1], randomWeight);
			//je¿eli jest to graf nieskierowany (algorytm MST)
			if (this->algorithmType == AlgorithmType::MST) {
				//to macierz symetryczna wzglêdem g³ównej przekatnej
				matrix->addEdge(edge[1], edge[0], randomWeight);
				list->addEdge(edge[1], edge[0], randomWeight);
			}
			//usuniêcie krawêdzi z listy
			delete[] edge;
			poslibleEdges->pop(randomEdge);
			//zmniejszenie iloœci krawêdzi do wygenerowania
			toGenerate--;
		}
	}
	//zwolnienie pamiêci dla wszystkich elementów listy
	poslibleEdges->setItHead();
	size_t* el;
	while ((el = poslibleEdges->getNext())!= nullptr) {
		delete[] el;
	}
	//usuniêcie listy
	delete poslibleEdges;
	//ustawienia wierzcho³ka startowego je¿eli algorytm najkrótszej œcierzki lub przep³ywu
	if (this->algorithmType == AlgorithmType::shortestPath || this->algorithmType == AlgorithmType::maximumFlow) {
		size_t randomNode = std::rand() % nodesLength;
		this->matrix->setNodeStart(randomNode);
		this->list->setNodeStart(randomNode);
		//Ustawienie wierzcho³ka startowego dla algorytmu przep³ywu
		if (this->algorithmType == AlgorithmType::maximumFlow) {
			size_t randomNode2 = randomNode;
			//je¿eli macierz nie zawiera tylko jednego wierzcho³ka to musi byæ inny wierzcho³ek koñcowy ni¿ startowy
			while (nodesLength > 1 && randomNode2 == randomNode) {
				randomNode2 = std::rand() % nodesLength;
			}
			this->matrix->setNodeEnd(randomNode2);
			this->list->setNodeEnd(randomNode2);
		}
	}
}

//cin dla liczby size_t z sprawdzaniem poprawnoœci
size_t Menu::cinSize_t()
{
	size_t value;
	std::cin >> value;
	while (std::cin.good() == false)
	{
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cout << "Podaj poprawna wartosc indeksu: ";
		std::cin >> value;
	}
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	return value;
}
//cin dla liczby typu int w przedziale <0;max>
int Menu::cinuInt(int max)
{
	int value;
	std::cin >> value;
	while (std::cin.good() == false || value>max || value<2)
	{
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cout << "Podaj wartosc calkowita w przedziale <2;"+std::to_string(max)+">: ";
		std::cin >> value;
	}
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	return value;
}
//wybór operacji wed³ug wybranucj opcji w menu
void Menu::handleInput()
{
	//w zaleznosci od strony sa 4(0-3) opcje albo 6 (0-5)
	int items = 3;
	if (this->algorithmChoosen) {
		items = 5;
	}
	//obs³uga klawiszy w,s
	switch (this->input)
	{
	case 119:
		if (this->option <= 0) {
			this->option = items;
		}
		else {
			this->option--;
		}
		break;
	case 115:
		if (this->option >= items) {
			this->option = 0;
		}
		else {
			this->option++;
		}
		break;
	//przycisk enter
	case 13:
		{
			system("CLS");
			//obs³uga wybranego elementu w zale¿noœæi od poziomu menu
			if (this->algorithmChoosen) {
				switch (this->option)
				{
				//obs³uga pliku
				case 0:
					{
						//wczytywanie nazwy pliku
						std::string fileName;
						std::cout << "Indeksowanie wierzcholkow musi zaczynac sie od 0!\n" << "Podaj nazwe pliku : ";
						std::getline(std::cin, fileName);
						//otwieranie pliku
						std::fstream* file = new std::fstream(fileName, std::ios::in);
						//jezeli udalo wczytac sie plik to wypisz dane, jezeli nie to blad
						if (this->loadFromFile(file)) {
							std::cout << "[ Wczytane dane ]: \n\n";
							this->matrix->display();
							std::cout << "\n\n";
							this->list->display();
						}
						else {
							this->matrix->erase();
							this->list->erase();
							std::cout << "Blad podczas otwierania pliku.\n\n";
						}
						delete file;
					}
					break;
				//obsluga losowania wartosci grafu
				case 1:
					{
						std::cout << "Podaj ilosc wierzcholokow: ";
						size_t nodes = Menu::cinSize_t();
						std::cout << "Podaj stopien wypelnienia: ";
						int fill = Menu::cinuInt(100);
						this->random(nodes, fill);
						std::cout << "\n\n[ Wygenerowano dane ]:\n\n";
						this->matrix->display();
						std::cout << "\n\n";
						this->list->display();
						break;
					}
				//obsluga algorytmow pierwszego typu
				case 2:
					if (this->matrix->isActive && this->list->isActive) {
						switch (this->algorithmType)
						{
						case AlgorithmType::MST:
							{
								//Prim dla macierzy sasiedztwa
								SpanningTree* returnPrim = this->matrix->prim();
								std::cout << "Algorytm Prima dla macierzy sasiedztwa:\n";
								returnPrim->display();
								delete returnPrim;
								std::cout << "\n\n";
								//Prim dla listy sasiedztwa
								returnPrim = this->list->prim();
								std::cout << "Algorytm Prima dla listy sasiedztwa:\n";
								returnPrim->display();
								delete returnPrim;
								break;
							}
						case AlgorithmType::shortestPath:
							{
								//jezeli wczesniej nie byl wykonano algorytm dijkstry
								if (this->list->negativValues || this->matrix->negativValues) {
									std::cout << "Algorytm dijkstry wymaga nieujemnych wartosci. Wszystkie ujemne wartosci zostana zamienione na dodatnie.\n Chcesz kontynuowac? [enter]. nacisnij dowolny klawisz aby przerwac.\n\n";
									if (_getch() == 13) {
										this->list->makeNonNegative();
										this->list->negativValues = false;
										this->matrix->makeNonNegative();
										this->matrix->negativValues = false;
									}
									else {
										break;
									}


								}

								//Dijkstra dla macierzy sasiedztwa
								ShortestPath* returnDijkstra = this->matrix->dijkstra();
								std::cout << "Algorytm Dijkstry dla macierzy sasiedztwa:\n";
								returnDijkstra->display();
								delete returnDijkstra;
								std::cout << "\n\n";
								//Dijkstra dla listy sasiedztwa
								returnDijkstra = this->matrix->dijkstra();
								std::cout << "Algorytm Dijkstry dla listy sasiedztwa:\n";
								returnDijkstra->display();
								delete returnDijkstra;
							}
							break;
						case AlgorithmType::maximumFlow:
							std::cout << "Przeplyw z [ " << std::to_string(this->matrix->nodeStart) << " ] do [ " << std::to_string(this->matrix->nodeEnd) << " ]\n";
							//upewnienie siê, ¿e wartoœci ujemne
							this->list->makeNonNegative();
							this->list->negativValues = false;
							this->matrix->makeNonNegative();
							this->matrix->negativValues = false;
							//Algorytm Forda Fulkersona dla macierzy sasiedztwa
							int flow = this->matrix->fordFulkerson();
							if (flow > 0) {
								std::cout << "Maksymalny przeplyw dla macierzy sasiedztwa: " << flow << "\n";
								//Algorytm Forda Fulkersona dla listy sasiedztwa
								flow = this->list->fordFulkerson();
								std::cout << "Maksymalny przeplyw dla listy sasiedztwa: " << flow << "\n";
							}
							else {
								std::cout << "Nie znaleziono polaczenia miedzy wierzcholkami\n";
							}
							break;
						}
					}
					break;
				//obsluga algorytmow drugiego typu
				case 3:
					if (this->matrix->isActive && this->list->isActive) {
						switch (this->algorithmType)
						{
						case AlgorithmType::MST:
						{
							//Kruskal dla macierzy sasiedztwa
							SpanningTree* returnKruskal = this->matrix->kruskal();
							std::cout << "Algorytm Kruskala dla macierzy sasiedztwa:\n";
							returnKruskal->display();
							delete returnKruskal;
							std::cout << "\n\n";
							//Kruskal dla listy sasiedztwa
							returnKruskal = this->list->kruskal();
							std::cout << "Algorytm Kruskala dla listy sasiedztwa:\n";
							returnKruskal->display();
							delete returnKruskal;
							break;
						}
						case AlgorithmType::shortestPath:
						{
							//jezeli wczesniej wykonano algorytm dijkstry
							if (!this->list->negativValues || !this->matrix->negativValues) {
								std::cout << "Uwaga, wczesniej uzyty byl algorytm dijkstry.\n Jezeli chcesz aby wartosci byly ujemne, wczytaj ponowanie dane\n nacisnij dowolny klawisz by kontynuowac...\n\n";
								char test = _getch();
							}
							//Bellman-Ford dla macierzy sasiedztwa
							ShortestPath* returnBellmanFord = this->matrix->bellmanFord();
							std::cout << "Algorytm Bellmana-Forda dla macierzy sasiedztwa:\n";
							if (returnBellmanFord->signedValues) {
								std::cout << "Wykryto przebieg ujemnu.\n Kazda sciezka moglaby byc nieskonczenie mala, gdyby przepuscic ja nieskonczenie wiele razy przez przebieg ujemny\n";
								std::cout << " Sciezka nie zostanie wyswietlona, jednak zostala obliczona, gdyby byla potrzebna do obliczen";
							}
							else {
								returnBellmanFord->display();
							}
							delete returnBellmanFord;
							std::cout << "\n\n";
							returnBellmanFord = this->list->bellmanFord();
							std::cout << "Algorytm Bellmana-Forda dla listy sasiedztwa:\n";
							if (returnBellmanFord->signedValues) {
								std::cout << "Wykryto przebieg ujemnu.\n Kazda sciezka moglaby byc nieskonczenie mala, gdyby przepuscic ja nieskonczenie wiele razy przez przebieg ujemny\n";
								std::cout << " Sciezka nie zostanie wyswietlona, jednak zostala obliczona, gdyby byla potrzebna do obliczen";
							}
							else {
								returnBellmanFord->display();
							}
							delete returnBellmanFord;
							break;
						}
						case AlgorithmType::maximumFlow:
							//nie ma drugiego algorytmu
							break;
						}
					}
					break;
				//obsluga rysowania grafu
				case 4:
					if (this->matrix->isActive && this->list->isActive) {
						this->matrix->display();
						std::cout << "\n\n";
						this->list->display();
					}
					break;
				//powrot do wyboru algorytmu
				case 5:
					this->algorithmChoosen = false;
					this->matrix->erase();
					this->list->erase();
					this->option = 0;
					break;
				}
				std::cout << "\n\n";
				system("PAUSE");
			}
			else {
				//wybor alogrytmu
				switch (this->option)
				{
				case 0:
					this->algorithmChoosen = true;
					this->algorithmType = AlgorithmType::MST;
					break;
				case 1:
					this->algorithmChoosen = true;
					this->algorithmType = AlgorithmType::shortestPath;
					break;
				case 2:
					this->algorithmChoosen = true;
					this->algorithmType = AlgorithmType::maximumFlow;
					break;
				case 3:
					this->test();
					system("PAUSE");
					break;
				}
				this->option = 0;
			}
			break;
		}
	}
}

//rysowanie menu w zale¿noœci, czy wybrany zosta³ algorytm, czy nie
void Menu::drawMenu()
{
	std::string information = "";
	//informacje u góry menu
	information+=INFORMATION;
	//sprawdzanie poziomu menu
	if (this->algorithmChoosen) {
		//wypisanie wybranego algorytmu
		switch (this->algorithmType)
		{
		case AlgorithmType::MST:
			information += "              [ MST ]";
			break;
		case AlgorithmType::shortestPath:
			information += "       [ Najkrotsza sciezka ]";
			break;
		case AlgorithmType::maximumFlow:
			information += "      [ Maksymalny przeplyw ]";
			break;
		}
		information+="\n\n";
		//rysowanie elementow w menu i w zaleznosci czy jest wyrozniony rysowanie szczalek
		if (this->option == 0) {
			information += "    ->";
		}
		else {
			information += "      ";
		}
		information += "Wczytaj z pliku\n";
		if (this->option == 1) {
			information += "    ->";
		}
		else {
			information += "      ";
		}
		information += "Wygeneruj graf losowo\n";
		if (this->option == 2) {
			information += "    ->";
		}
		else {
			information += "      ";
		}
		if (this->matrix->isActive && this->list->isActive) {
			//w zaleznosci od typu algorytmu rozne algorytmy
			switch (this->algorithmType)
			{
			case AlgorithmType::MST:
				information += "Algorytm Prima\n";
				break;
			case AlgorithmType::shortestPath:
				information += "Algorytm Dijkstry\n";
				break;
			case AlgorithmType::maximumFlow:
				information += "Algorytm Forda-Fulkersona\n";
				break;
			}
		}
		else {
			information += "-----------\n";
		}
		if (this->option == 3) {
			information += "    ->";
		}
		else {
			information += "      ";
		}
		if (this->matrix->isActive && this->list->isActive) {
			//w zaleznosci od typu algorytmu rozne algorytmy
			switch (this->algorithmType)
			{
			case AlgorithmType::MST:
				information += "Algorytm Kruskala\n";
				break;
			case AlgorithmType::shortestPath:
				information += "Algorytm Forda-Bellmana\n";
				break;
			case AlgorithmType::maximumFlow:
				information += "-----------\n";
				break;
			}
		}
		else {
			information += "-----------\n";
		}
		if (this->option == 4) {
			information += "    ->";
		}
		else {
			information += "      ";
		}
		if (this->matrix->isActive && this->list->isActive) {
			information += "Wyswietl graf w postaci listy i macierzy\n";
		}
		else {
			information += "-----------\n";
		}
		if (this->option == 5) {
			information += "    ->";
		}
		else {
			information += "      ";
		}
		information += "Powrot\n";
	}
	else {
		//rysowanie menu dla wyboru algorytmu
		information+="\n\n";
		if (this->option == 0) {
			information += "    ->";
		}
		else {
			information += "      ";
		}
		information += "Minimalne drzewo rozpinajace\n";
		if (this->option == 1) {
			information += "    ->";
		}
		else {
			information += "      ";
		}
		information += "Najkrotsza sciezka w grafie\n";
		if (this->option == 2) {
			information += "    ->";
		}
		else {
			information += "      ";
		}
		information += "Maksymalny przeplyw\n";
		if (this->option == 3) {
			information += "    ->";
		}
		else {
			information += "      ";
		}
		information += "Testy\n";
	}

	std::cout << information;
}
//testy iloœci czasu na strukturach
void Menu::test()
{
	std::cout << "Podaj ilosc wierzcholkow: ";
	size_t nodesLength = Menu::cinSize_t();
	TimeMeasure* measure = new TimeMeasure();
	std::cout << "Czasy w us:\n\n";
	std::cout << "Wypelnienie 20%:\n";
	std::cout << "-Algorytm Prima:\n";
	long long timeMatrix=0;
	long long timeList=0;
	this->algorithmType = AlgorithmType::MST;
	for (int i = 0; i < 100; i++) {
		this->random(nodesLength, 20);
		measure->restart();
		this->matrix->prim();
		timeMatrix += measure->returnTime();
		measure->restart();
		this->list->prim();
		timeList += measure->returnTime();
	}
	std::cout << "Macierz sasiedztwa: " << static_cast<double>(timeMatrix)/100 << "\n";
	std::cout << "Lista sasiedztwa  : " << static_cast<double>(timeList) / 100 << "\n\n";
	std::cout << "-Algorytm Kruskala:\n";
	timeMatrix = 0;
	timeList = 0;
	this->algorithmType = AlgorithmType::MST;
	for (int i = 0; i < 100; i++) {
		this->random(nodesLength, 20);
		measure->restart();
		this->matrix->kruskal();
		timeMatrix += measure->returnTime();
		measure->restart();
		this->list->kruskal();
		timeList += measure->returnTime();
	}
	std::cout << "Macierz sasiedztwa: " << static_cast<double>(timeMatrix) / 100 << "\n";
	std::cout << "Lista sasiedztwa  : " << static_cast<double>(timeList) / 100 << "\n\n";
	std::cout << "-Algorytm Dijkstry:\n";
	timeMatrix = 0;
	timeList = 0;
	this->algorithmType = AlgorithmType::shortestPath;
	for (int i = 0; i < 100; i++) {
		this->random(nodesLength, 20);
		measure->restart();
		this->matrix->dijkstra();
		timeMatrix += measure->returnTime();
		measure->restart();
		this->list->dijkstra();
		timeList += measure->returnTime();
	}
	std::cout << "Macierz sasiedztwa: " << static_cast<double>(timeMatrix) / 100 << "\n";
	std::cout << "Lista sasiedztwa  : " << static_cast<double>(timeList) / 100 << "\n\n";
	std::cout << "-Algorytm Forda-Bellmana:\n";
	timeMatrix = 0;
	timeList = 0;
	this->algorithmType = AlgorithmType::shortestPath;
	for (int i = 0; i < 100; i++) {
		this->random(nodesLength, 20);
		measure->restart();
		this->matrix->bellmanFord();
		timeMatrix += measure->returnTime();
		measure->restart();
		this->list->bellmanFord();
		timeList += measure->returnTime();
	}
	std::cout << "Macierz sasiedztwa: " << static_cast<double>(timeMatrix) / 100 << "\n";
	std::cout << "Lista sasiedztwa  : " << static_cast<double>(timeList) / 100 << "\n\n";
	std::cout << "-Algorytm Forda Fulkersona:\n";
	timeMatrix = 0;
	timeList = 0;
	this->algorithmType = AlgorithmType::maximumFlow;
	for (int i = 0; i < 100; i++) {
		this->random(nodesLength, 20);
		measure->restart();
		this->matrix->fordFulkerson();
		timeMatrix += measure->returnTime();
		measure->restart();
		this->list->fordFulkerson();
		timeList += measure->returnTime();
	}
	std::cout << "Macierz sasiedztwa: " << static_cast<double>(timeMatrix) / 100 << "\n";
	std::cout << "Lista sasiedztwa  : " << static_cast<double>(timeList) / 100 << "\n\n\n";
	std::cout << "Wypelnienie 60%:\n";
	std::cout << "-Algorytm Prima:\n";
	timeMatrix = 0;
	timeList = 0;
	this->algorithmType = AlgorithmType::MST;
	for (int i = 0; i < 100; i++) {
		this->random(nodesLength, 60);
		measure->restart();
		this->matrix->prim();
		timeMatrix += measure->returnTime();
		measure->restart();
		this->list->prim();
		timeList += measure->returnTime();
	}
	std::cout << "Macierz sasiedztwa: " << static_cast<double>(timeMatrix) / 100 << "\n";
	std::cout << "Lista sasiedztwa  : " << static_cast<double>(timeList) / 100 << "\n\n";
	std::cout << "-Algorytm Kruskala:\n";
	timeMatrix = 0;
	timeList = 0;
	this->algorithmType = AlgorithmType::MST;
	for (int i = 0; i < 100; i++) {
		this->random(nodesLength, 60);
		measure->restart();
		this->matrix->kruskal();
		timeMatrix += measure->returnTime();
		measure->restart();
		this->list->kruskal();
		timeList += measure->returnTime();
	}
	std::cout << "Macierz sasiedztwa: " << static_cast<double>(timeMatrix) / 100 << "\n";
	std::cout << "Lista sasiedztwa  : " << static_cast<double>(timeList) / 100 << "\n\n";
	std::cout << "-Algorytm Dijkstry:\n";
	timeMatrix = 0;
	timeList = 0;
	this->algorithmType = AlgorithmType::shortestPath;
	for (int i = 0; i < 100; i++) {
		this->random(nodesLength, 60);
		measure->restart();
		this->matrix->dijkstra();
		timeMatrix += measure->returnTime();
		measure->restart();
		this->list->dijkstra();
		timeList += measure->returnTime();
	}
	std::cout << "Macierz sasiedztwa: " << static_cast<double>(timeMatrix) / 100 << "\n";
	std::cout << "Lista sasiedztwa  : " << static_cast<double>(timeList) / 100 << "\n\n";
	std::cout << "-Algorytm Forda-Bellmana:\n";
	timeMatrix = 0;
	timeList = 0;
	this->algorithmType = AlgorithmType::shortestPath;
	for (int i = 0; i < 100; i++) {
		this->random(nodesLength, 60);
		measure->restart();
		this->matrix->bellmanFord();
		timeMatrix += measure->returnTime();
		measure->restart();
		this->list->bellmanFord();
		timeList += measure->returnTime();
	}
	std::cout << "Macierz sasiedztwa: " << static_cast<double>(timeMatrix) / 100 << "\n";
	std::cout << "Lista sasiedztwa  : " << static_cast<double>(timeList) / 100 << "\n\n";
	std::cout << "-Algorytm Forda Fulkersona:\n";
	timeMatrix = 0;
	timeList = 0;
	this->algorithmType = AlgorithmType::maximumFlow;
	for (int i = 0; i < 100; i++) {
		this->random(nodesLength, 60);
		measure->restart();
		this->matrix->fordFulkerson();
		timeMatrix += measure->returnTime();
		measure->restart();
		this->list->fordFulkerson();
		timeList += measure->returnTime();
	}
	std::cout << "Macierz sasiedztwa: " << static_cast<double>(timeMatrix) / 100 << "\n";
	std::cout << "Lista sasiedztwa  : " << static_cast<double>(timeList) / 100 << "\n\n\n";
	std::cout << "Wypelnienie 99%:\n";
	std::cout << "-Algorytm Prima:\n";
	timeMatrix = 0;
	timeList = 0;
	this->algorithmType = AlgorithmType::MST;
	for (int i = 0; i < 100; i++) {
		this->random(nodesLength, 99);
		measure->restart();
		this->matrix->prim();
		timeMatrix += measure->returnTime();
		measure->restart();
		this->list->prim();
		timeList += measure->returnTime();
	}
	std::cout << "Macierz sasiedztwa: " << static_cast<double>(timeMatrix) / 100 << "\n";
	std::cout << "Lista sasiedztwa  : " << static_cast<double>(timeList) / 100 << "\n\n";
	std::cout << "-Algorytm Kruskala:\n";
	timeMatrix = 0;
	timeList = 0;
	this->algorithmType = AlgorithmType::MST;
	for (int i = 0; i < 100; i++) {
		this->random(nodesLength, 99);
		measure->restart();
		this->matrix->kruskal();
		timeMatrix += measure->returnTime();
		measure->restart();
		this->list->kruskal();
		timeList += measure->returnTime();
	}
	std::cout << "Macierz sasiedztwa: " << static_cast<double>(timeMatrix) / 100 << "\n";
	std::cout << "Lista sasiedztwa  : " << static_cast<double>(timeList) / 100 << "\n\n";
	std::cout << "-Algorytm Dijkstry:\n";
	timeMatrix = 0;
	timeList = 0;
	this->algorithmType = AlgorithmType::shortestPath;
	for (int i = 0; i < 100; i++) {
		this->random(nodesLength, 99);
		measure->restart();
		this->matrix->dijkstra();
		timeMatrix += measure->returnTime();
		measure->restart();
		this->list->dijkstra();
		timeList += measure->returnTime();
	}
	std::cout << "Macierz sasiedztwa: " << static_cast<double>(timeMatrix) / 100 << "\n";
	std::cout << "Lista sasiedztwa  : " << static_cast<double>(timeList) / 100 << "\n\n";
	std::cout << "-Algorytm Forda-Bellmana:\n";
	timeMatrix = 0;
	timeList = 0;
	this->algorithmType = AlgorithmType::shortestPath;
	for (int i = 0; i < 100; i++) {
		this->random(nodesLength, 99);
		measure->restart();
		this->matrix->bellmanFord();
		timeMatrix += measure->returnTime();
		measure->restart();
		this->list->bellmanFord();
		timeList += measure->returnTime();
	}
	std::cout << "Macierz sasiedztwa: " << static_cast<double>(timeMatrix) / 100 << "\n";
	std::cout << "Lista sasiedztwa  : " << static_cast<double>(timeList) / 100 << "\n\n";
	std::cout << "-Algorytm Forda Fulkersona:\n";
	timeMatrix = 0;
	timeList = 0;
	this->algorithmType = AlgorithmType::maximumFlow;
	for (int i = 0; i < 100; i++) {
		this->random(nodesLength, 99);
		measure->restart();
		this->matrix->fordFulkerson();
		timeMatrix += measure->returnTime();
		measure->restart();
		this->list->fordFulkerson();
		timeList += measure->returnTime();
	}
	std::cout << "Macierz sasiedztwa: " << static_cast<double>(timeMatrix) / 100 << "\n";
	std::cout << "Lista sasiedztwa  : " << static_cast<double>(timeList) / 100 << "\n\n";
	delete measure;
}

bool Menu::loadFromFile(std::fstream* file)
{
	if (file->good()) {
		try {
			std::string line;
				
			std::getline(*file, line);
			//pozycha spacji
			size_t pos = line.find(' ');
			//pozycja poczatku liczby
			size_t initialPos = 0;
			//liczba krawêdzi
			size_t edgesLength = static_cast<size_t>(std::stoi(line.substr(initialPos, pos - initialPos)));
			//iloœæ wierzcho³ków
			initialPos = pos + 1;
			pos = line.find(' ', initialPos);
			size_t nodesLength = static_cast<size_t>(std::stoi(line.substr(initialPos, pos - initialPos)));
			//tworzenie struktur
			this->matrix->create(nodesLength, edgesLength);
			this->list->create(nodesLength, edgesLength);
			//ustawienia wierzcho³ka startowego je¿eli algorytm najkrótszej œcierzki lub przep³ywu
			if (this->algorithmType == AlgorithmType::shortestPath || this->algorithmType == AlgorithmType::maximumFlow) {
				initialPos = pos + 1;
				pos = line.find(' ', initialPos);
				size_t nodeStart = static_cast<size_t>(std::stoi(line.substr(initialPos, pos - initialPos)));
				this->matrix->setNodeStart(nodeStart);
				this->list->setNodeStart(nodeStart);
				//Ustawienie wierzcho³ka startowego dla algorytmu przep³ywu
				if (this->algorithmType == AlgorithmType::maximumFlow) {
					initialPos = pos + 1;
					pos = line.find(' ', initialPos);
					size_t nodeEnd = static_cast<size_t>(std::stoi(line.substr(initialPos, pos - initialPos)));
					this->matrix->setNodeEnd(nodeEnd);
					this->list->setNodeEnd(nodeEnd);
				}
			}
			//Wczytanie krawêdzi wed³ug podanej wielkoœci
			size_t countEdes = 0;
			for (int i = 0; i < edgesLength && !file->eof(); i++) {
				std::getline(*file, line);
				//Wierzcho³ek poczatkowy
				pos = line.find(' ');
				initialPos = 0;
				size_t nodeFrom = static_cast<size_t>(std::stoi(line.substr(initialPos, pos - initialPos)));
				//Wierzcho³ek koñcowy
				initialPos = pos + 1;
				pos = line.find(' ', initialPos);
				size_t nodeTo = static_cast<size_t>(std::stoi(line.substr(initialPos, pos - initialPos)));
				//Waga
				initialPos = pos + 1;
				pos = line.find(' ', initialPos);
				int weight = std::stoi(line.substr(initialPos, pos - initialPos));
				//jezeli wierzcholki mieszcza sie w zakresie
				if (nodeFrom < nodesLength && nodeFrom >= 0 && nodeTo < nodesLength && nodeTo >= 0) {
					this->matrix->addEdge(nodeFrom, nodeTo, weight);
					this->list->addEdge(nodeFrom, nodeTo, weight);
					//Je¿eli typ algorytmu mst to krawêdzie równie¿ w druga stronê
					if (this->algorithmType == AlgorithmType::MST) {
						this->matrix->addEdge(nodeTo, nodeFrom, weight);
						this->list->addEdge(nodeTo, nodeFrom, weight);
					}
					countEdes++;
				}
				else {
					std::cout << "Wierzcholek ma inny indeks niz przedzial <0;" + std::to_string(nodesLength-1) + ">\n";
					return false;
				}
			}
			if (edgesLength != countEdes) {
				std::cout << "Niepoprawna liczba krawedzi w stosunku do zadeklarowanej\n";
				return false;
			}
			return true;
		}
		catch (std::exception) {
			std::cout << "Plik zawiera niepoprawne dane (moga to byc inne znaki niz cyfry)\n";
		}
	}
	std::cout << "Nie udalo sie otworzyc pliku\n";
	return false;
}

Menu::Menu()
{
	this->matrix = new AdjacencyMatrix();
	this->list = new AdjacencyList();
	this->input = 0;
	this->algorithmChoosen = false;
	this->algorithmType = AlgorithmType::MST;
	this->option = 0;
}

int Menu::numberLength(long number)
{
	int length = 0;
	//je¿eli liczba ujemna to dodatkowe miejsce na znak i sprawienie by by³a dodatnia
	if (number < 0) {
		number *= -1;
		length++;
	}
	//dopuki liczba jest wiêksza od 0 wykonuj dzielenie i zwiêksz licznik d³ugoœci liczby
	do {
		number /= 10;
		length++;
	} while (number > 0);
	return length;
}

//petla glowna
void Menu::loop()
{
	//wyjœcie z pêtli je¿eli klawisz to ecape
	while (input != 27) {
		//obsluga klawiszy
		this->handleInput();
		//Czysczenie ekranu
		system("CLS");
		//rysowanie menu
		this->drawMenu();
		//pobranie klawisza
		this->input = _getch();
	}
}

Menu::~Menu()
{
	delete this->matrix;
	delete this->list;
}
