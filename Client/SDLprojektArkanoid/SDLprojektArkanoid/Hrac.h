#pragma once
#include<SDL.h>
#include "heap_monitor.h"
#include "Constatns.h"

class Hrac
{
private:
	const int VYSKA = 20;
	int sirkaHraca = 100;
	const int INIT_X = (Constants::SIRKA) / 2 - sirkaHraca / 2;
	int farbaHraca[3] = { 255, 0, 0 };

	//const int INIT_Y = (Constants::VYSKA) - VYSKA;

	SDL_Rect obdlznik;
public:
	Hrac();
	int getPolohaX(); //vrati polohu x
	int getPolohaY();
	int getSirka();
	int getVyska();
	int getFarba(int param); //vrati farbu podla parametru je to lahsie 

	void setPolohaX(int x); //nastavi polohu x
	//void setPolohaY(int y);
	void setFarbuHraca(int r, int g, int b);
	void setInitY();
	void setInitY(int y);
	void setInitX(int x);

	void zmenSirku(int parLevel);

	SDL_Rect getSeba();

	~Hrac();
};

