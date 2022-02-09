#include "Prekazka.h"



Prekazka::Prekazka(int i, int j)
{
	this->prekazka.w = this->SIRKA;
	this->prekazka.h = this->VYSKA;
	this->prekazka.x = Constants::SIRKA_PREKAZKY_RATANIE / 2 + (i * Constants::SIRKA_PREKAZKY_RATANIE);

	this->prekazka.y = Constants::VYSKA_PREKAZKY + (j * Constants::VYSKA_PREKAZKY_RATANIE);
	this->zobrazena = true;//rand() % 3;
}

SDL_Rect Prekazka::getSeba()
{
	return this->prekazka;
}

int Prekazka::getPolohaX()
{
	return this->prekazka.x;
}

int Prekazka::getPolohaY()
{
	return this->prekazka.y;
}

int Prekazka::getSirku()
{
	return this->SIRKA;
}

int Prekazka::getVysku()
{
	return this->VYSKA;
}

void Prekazka::setZobrazena(bool zobrazena)
{
	this->zobrazena = zobrazena;
}


bool Prekazka::siZobreazena()
{
	return this->zobrazena;
}

Prekazka::~Prekazka()
{
}
