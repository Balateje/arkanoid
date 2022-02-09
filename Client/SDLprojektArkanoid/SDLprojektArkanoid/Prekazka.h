#pragma once
#include <SDL.h>
#include "Constatns.h"
#include "heap_monitor.h"
#include <random>


class Prekazka
{
	SDL_Rect prekazka;
	const int VYSKA = Constants::VYSKA_PREKAZKY;
	const int SIRKA = Constants::SIRKA_PREKAZKY;
	bool zobrazena;
	//const int INIT_X = (Constants::SIRKA) / 2 - SIRKA / 2;
	//const int INIT_Y = (Constants::VYSKA) - VYSKA + 20; // zobrazenie prekazky 

public:
	Prekazka(int i, int j);
	SDL_Rect getSeba();
	int getPolohaX(); //vrati polohu x
	int getPolohaY();
	int getSirku();
	int getVysku();
	void setZobrazena(bool zobrazena);
	//void setPolohaY(int b);
	bool siZobreazena();

	
	~Prekazka();
};

