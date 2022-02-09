#pragma once
#include <SDL.h>
#include <random>
#include "heap_monitor.h"
#include "Constatns.h"
#include <time.h> 


class Lopta

{
private:
	int rychlost = 5;
	const int VYSKA = 15;
	const int SIRKA = 15;
	const int INIT_X = (Constants::SIRKA) / 2 - SIRKA / 2;
	const int INIT_Y = (Constants::VYSKA) - VYSKA * 20; // zobrazenie lopty
	SDL_Rect lopta;
	int deltaX, deltaY;

public:
	Lopta();
	int counter = 0;
	int getPolohaX(); //vrati polohu x
	int getPolohaY();
	int getSirku();
	int getVysku();
	int getDeltaY();
	int getDeltaX();
	void setDeltaX(); //nastavi polohu x
	void setDeltaY();
	void setPolohaX(int paramX);
	void setPolohaY(int paramY);
	void zrychliLopticku(int parLevel);
	void krok();
	void uder();
	void uderPrekazky(bool smer);
	SDL_Rect getSeba();

	~Lopta();
};

