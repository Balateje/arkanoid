#pragma once
#include<SDL.h>
#include "heap_monitor.h"
#include "Constatns.h"


class Menu
{
private:
	const int VYSKA = 50;
	const int SIRKA = 140;
	const int INIT_X = (Constants::SIRKA) / 2 - SIRKA / 2 - 50;
	//const int INIT_Y = (Constants::VYSKA) - VYSKA;

	SDL_Rect startOBD; //start oblznik
	SDL_Rect skoreOBD; //vyppis skore oblznik
	SDL_Rect koniecOBD; //koniec oblznik
	SDL_Rect levelOBD; //vyppis vitazny oblznik


	bool start = false;
	//bool skore = false;
	bool koniec = false;


public:
	Menu();

	SDL_Rect getStart();
	SDL_Rect getSkore();
	SDL_Rect getKoniec();
	SDL_Rect getLevel();
	int getX();
	int getY();
	int getSirka();
	int getVyska();


	~Menu();
};
