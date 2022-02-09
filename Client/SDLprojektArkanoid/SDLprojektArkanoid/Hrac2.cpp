#include "Hrac2.h"



Hrac2::Hrac2()
{

	this->obdlznik.x = this->INIT_X;
	this->obdlznik.y = this->INIT_Y;
	this->obdlznik.w = this->SIRKA;
	this->obdlznik.h = this->VYSKA;
}

int Hrac2::getPolohaX()
{
	return 	this->obdlznik.x;
}

int Hrac2::getPolohaY()
{
	return 	this->obdlznik.y;
}

int Hrac2::getSirka()
{
	return this->SIRKA;
}

int Hrac2::getVyska()
{
	return this->VYSKA;
}


void Hrac2::setPolohaX(int x)
{
	if (((this->obdlznik.x + x) <= Constants::SIRKA - this->SIRKA) && (this->obdlznik.x + x) >= (this->SIRKA - this->obdlznik.w)) {
		// obmedzenie v pohybe zo strany na stranu
		this->obdlznik.x += x;
	}
}

void Hrac2::setPolohaY(int y)
{
	if (((this->obdlznik.y + y) <= Constants::VYSKA - this->VYSKA) && ((this->obdlznik.y + y) >= (Constants::VYSKA - this->VYSKA - this->obdlznik.h))) {
		//hore sa moze posunut iba o svoju vysku
		this->obdlznik.y += y;
	}

}

SDL_Rect Hrac2::getSeba()
{
	return this->obdlznik;
}


Hrac2::~Hrac2()
{
	//vymazat obdlznik
}
