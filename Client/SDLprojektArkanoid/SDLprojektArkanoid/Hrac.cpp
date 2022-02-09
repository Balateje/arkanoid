#include "Hrac.h"



Hrac::Hrac()
{

	this->obdlznik.x = this->INIT_X;
	this->obdlznik.y = (Constants::VYSKA) - VYSKA;
	this->obdlznik.w = this->sirkaHraca;
	this->obdlznik.h = this->VYSKA;
}

int Hrac::getPolohaX()
{
	return 	this->obdlznik.x;
}

int Hrac::getPolohaY()
{
	return 	this->obdlznik.y;
}

int Hrac::getSirka()
{
	return this->sirkaHraca;
}

int Hrac::getVyska()
{
	return this->VYSKA;
}

int  Hrac::getFarba(int param)
{
	if (param >= 0 && param <= 2) {
		return this->farbaHraca[param];
	}
	return 0;
}




void Hrac::setPolohaX(int x)
{
	if (((this->obdlznik.x + x) <= Constants::SIRKA - this->sirkaHraca) && (this->obdlznik.x + x) >= (this->sirkaHraca - this->sirkaHraca)) {
		// obmedzenie v pohybe zo strany na stranu
		this->obdlznik.x += x;
	}
}
/*
void Hrac::setPolohaY(int y)
{
	if (((this->obdlznik.y + y) <= Constants::VYSKA - this->VYSKA) && ((this->obdlznik.y + y) >= (Constants::VYSKA - this->VYSKA - this->obdlznik.h))) {
			//hore sa moze posunut iba o svoju vysku
		this->obdlznik.y += y;
	}
}
*/

void Hrac::setFarbuHraca(int r, int g, int b)
{
	this->farbaHraca[0] = r;
	this->farbaHraca[1] = g;
	this->farbaHraca[2] = b;
}

void Hrac::setInitY() //pre protihraca da ho vysie 
{
	this->obdlznik.y = (Constants::VYSKA) - VYSKA * 2 - 2;
}

void Hrac::setInitY(int y)
{
	this->obdlznik.y = y;
}

void Hrac::setInitX(int x)
{
	this->obdlznik.x = x;
}

void Hrac::zmenSirku(int parLevel)
{
	this->sirkaHraca = this->sirkaHraca / parLevel;
	this->obdlznik.w = this->sirkaHraca;
}

SDL_Rect Hrac::getSeba()
{
	return this->obdlznik;
}


Hrac::~Hrac()
{
	//vymazat obdlznik
}
