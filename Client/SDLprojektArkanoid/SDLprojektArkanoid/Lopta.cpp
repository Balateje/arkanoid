#include "Lopta.h"



Lopta::Lopta()
{
	srand(time(NULL));
	/*
	int randX = rand() % 3 - 1; //interval od -1 po 1
	int randY = rand() % 3 - 1;

	randY == 0 ? this->deltaY = 1 : this->deltaY = randY; //podmienka ak bude 0 nechda 1 inak by to slo vodorvne/vertikalne
	randX == 0 ? this->deltaX = 1 : this->deltaX = randX;

	*/
	this->deltaX = 1;
	this->deltaY = 1;
	this->lopta.x = this->INIT_X;
	this->lopta.y = this->INIT_Y;
	this->lopta.w = this->SIRKA;
	this->lopta.h = this->VYSKA;

}

int Lopta::getPolohaX()
{
	return this->lopta.x;
}

int Lopta::getPolohaY()
{
	return this->lopta.y;
}

int Lopta::getSirku()
{
	return this->SIRKA;
}

int Lopta::getVysku()
{
	return this->VYSKA;
}

int Lopta::getDeltaY()
{
	return this->deltaY;
}

int Lopta::getDeltaX()
{
	return this->deltaX;
}

void Lopta::setDeltaX()
{
	if (((this->lopta.x + this->deltaX) <= Constants::SIRKA - this->SIRKA) && (this->lopta.x + this->deltaX) >= 0) {
		// obmedzenie v pohybe zo strany na stranu
		this->lopta.x += this->deltaX;
	}
	else {
		this->deltaX *= (-1);
		this->lopta.x += this->deltaX;
	}
}

void Lopta::setDeltaY()
{
	if (((this->lopta.y + this->deltaY) >= 0)) {

		this->lopta.y += this->deltaY;
	}
	else {
		this->deltaY *= (-1);
		this->lopta.y += this->deltaY;
	}
}

void Lopta::setPolohaX(int paramX)
{
	this->lopta.x = paramX;
}

void Lopta::setPolohaY(int paramY)
{
	this->lopta.y = paramY;
}

void Lopta::zrychliLopticku(int parLevel)
{
	this->rychlost = parLevel - 1;
}

void Lopta::krok()
{
	if (this->counter % rychlost == 0) {
		this->setDeltaX();
		this->setDeltaY();
	}
	this->counter = (this->counter + 1) % rychlost + 1;
}

void Lopta::uder()
{
	this->deltaY = abs(this->deltaY) * (-1);
	this->setDeltaY();
}

void Lopta::uderPrekazky(bool smer)
{
	if (smer) {
		this->deltaY *= (-1);
	}
	else {
		this->deltaX *= (-1);
	}

	/*
	this->deltaY *= (-1);
	this->setPolohaY();
	this->deltaX *= (-1);
	this->setPolohaX();
	*/
}

SDL_Rect Lopta::getSeba()
{
	return this->lopta;
}


Lopta::~Lopta()
{

}
