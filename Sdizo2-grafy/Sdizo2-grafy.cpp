// Sdizo2-grafy.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
#include "Menu.h"


int main()
{
    Menu *menu = new Menu();

    menu->loop();

    delete menu;
}
