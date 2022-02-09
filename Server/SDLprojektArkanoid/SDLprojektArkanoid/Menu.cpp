#include "Menu.h"



Menu::Menu()
{
	this->startOBD.x = this->INIT_X;
	this->startOBD.y = this->VYSKA;
	this->startOBD.w = this->SIRKA + 50;
	this->startOBD.h = this->VYSKA;

	this->skoreOBD.x = this->INIT_X - this->SIRKA / 2;
	this->skoreOBD.y = this->VYSKA * 3;
	this->skoreOBD.w = this->SIRKA * 2;
	this->skoreOBD.h = this->VYSKA;

	this->koniecOBD.x = this->INIT_X;
	this->koniecOBD.y = this->VYSKA + 100;
	this->koniecOBD.w = this->SIRKA + 50;
	this->koniecOBD.h = this->VYSKA;

	this->levelOBD.x = this->INIT_X - this->SIRKA / 2;
	this->levelOBD.y = this->VYSKA;
	this->levelOBD.w = this->SIRKA * 2;
	this->levelOBD.h = this->VYSKA;
}

SDL_Rect Menu::getStart()
{
	return this->startOBD;
}

SDL_Rect Menu::getSkore()
{
	return this->skoreOBD;
}
SDL_Rect Menu::getKoniec()
{
	return this->koniecOBD;
}
SDL_Rect Menu::getLevel()
{
	return this->levelOBD;
}
int Menu::getX()
{
	return this->startOBD.x;
}

int Menu::getY()
{
	return this->startOBD.y;
}

int Menu::getSirka()
{
	return this->SIRKA;
}

int Menu::getVyska()
{
	return this->VYSKA;
}




Menu::~Menu()
{
}
