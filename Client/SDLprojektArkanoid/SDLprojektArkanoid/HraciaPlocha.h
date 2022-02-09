#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <SDL.h>
#include <SDL_ttf.h>
#include <random>
#include <iostream>
#include <string>
#include "heap_monitor.h"
#include "Hrac.h"
#include "Constatns.h"
#include "Lopta.h"
#include "Menu.h"
#include "Prekazka.h"

#include <thread>
#include <mutex>
#include <condition_variable>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include <string>
#include <sstream>
#include <iterator>

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

using namespace std;

class HraciaPlocha
{
private:


	SDL_Window* okno;
	Menu* menu;
	SDL_Renderer* renderer;
	SDL_Event* udalost;
	bool koniec, pauza;
	bool prehra = false;
	bool vyhra = false;
	int vyberAkcie;
	int skore;
	static const int prekazkyVriadku = /*1;*/ (((Constants::SIRKA - (Constants::SIRKA_PREKAZKY/2)) / (Constants::SIRKA_PREKAZKY_RATANIE))); // pocet prekazok
	static const int prekazkyVStlpci = /*1;*/ (Constants::VYSKA / Constants::VYSKA_PREKAZKY_RATANIE / 4);
	bool jeZobrazeneMenu;

	int skoreCelk = 0;
	Lopta* lopta;
	Hrac* hrac;
	Hrac* protihrac;
	Prekazka* prekazka[prekazkyVriadku][prekazkyVStlpci];
	mutex mut;
	//condition_variable cond;
	bool isServer;
	int level = 1;
	int __cdecl client();
	int __cdecl server();

	int nicNeposiela = 1234;

public:
	HraciaPlocha();


	void spusti(bool server);
	void vymazStaruHru();
	void vytvorNovuHru();
	void vytvorNovuHru(int dalsiLevel);
	bool kontrolaPrienikov(Lopta* lopta, Hrac* hrac);
	bool jePrehra(Lopta* lopta);
	bool jeVyhra(int parSkore);
	bool jeZasahPrekazky(Prekazka* prekazka, Lopta* lopta);
	bool prehraOdos = false;
	bool vyhraOdos = false;
	//bool vitazstvo = false;
	//int polePrekazok[prekazkyVriadku][prekazkyVStlpci];

	void odchytanieUdalostiLogFun();
	//void odoslanie(int x);
	//void odoslanie(int pole[prekazkyVriadku][prekazkyVStlpci]);

	void odoslanie(int loptaX, int loptaY, int hracX, int prekPoleI, int prekPoleJ, int skore, int vybAkc, int parPrehra, int parVyhra);
	SOCKET ConnectSocket = INVALID_SOCKET;
	void vlaknoPrijimanie();



	void vycistiOkno();
	void vykresliMenu();
	void vykresliHra(); // , Prekazka *prekazka
	void vykresliPrekazku(int i, int j);
	//void vykresliPrekazky(Prekazka *prekazka);
	void prekresli();
	//void skryTrafenu(Prekazka *prekazka);
	void skryTrafenuPrekazku(int i, int j);

	void vypisSkore(int parSkore);


	~HraciaPlocha();
};


