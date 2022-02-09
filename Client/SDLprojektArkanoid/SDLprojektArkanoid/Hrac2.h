#pragma once
#include<SDL.h>
#include "Constatns.h"

class Hrac2
{
private:
	const int VYSKA = 20;
	const int SIRKA = 100;
	const int INIT_X = (Constants::SIRKA) / 2 - SIRKA / 2;
	const int INIT_Y = (Constants::VYSKA) - VYSKA*2;

	SDL_Rect obdlznik;
public:
	Hrac2();
	int getPolohaX(); //vrati polohu x
	int getPolohaY();
	int getSirka();
	int getVyska();
	void setPolohaX(int x); //nastavi polohu x
	void setPolohaY(int y);

	SDL_Rect getSeba();

	~Hrac2();
};
