#include "HraciaPlocha.h"

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

HraciaPlocha::HraciaPlocha()
{
	//inicializovanie 
	this->okno = SDL_CreateWindow("Hlavne_Okno", 100, 100, Constants::SIRKA, Constants::VYSKA, 0);
	this->renderer = SDL_CreateRenderer(okno, -1, 0);

	this->udalost = new SDL_Event();
	this->menu = new Menu();
	//this->vytvorNovuHru();

	this->koniec = false;
	this->pauza = true;
	this->vyberAkcie = 0;
	this->skore = 0;
	this->jeZobrazeneMenu = false;

}

HraciaPlocha::~HraciaPlocha()
{
	this->vymazStaruHru();
	delete this->menu;

	//SDL_free(udalost);
	SDL_DropEvent(udalost);
	delete this->udalost;
	SDL_DestroyWindow(okno);
}


void HraciaPlocha::spusti(bool server)
{
	this->isServer = server;

	if (isServer) {
		this->server();
		this->koniec = false;
	}
	else {
		this->client();
		this->koniec = false;
	}

	//cyklus hry -- do kym nebude vypnutie krizikom a este asi aj obzlznik koniec ked tak dorobim
}

void HraciaPlocha::vymazStaruHru()
{
	delete lopta;
	delete hrac;
	delete protihrac;
	for (int i = 0; i < prekazkyVriadku; i++) {
		for (int j = 0; j < prekazkyVStlpci; j++) {
			delete prekazka[i][j];
		}
	}
}

void HraciaPlocha::vytvorNovuHru()
{
	lopta = new Lopta();
	hrac = new Hrac();
	protihrac = new Hrac();
	protihrac->setInitY();

	if (this->isServer) { // ak je sserver tak vymeni kvazy hracov aby sa to zobrazovalo srpavne ak by toto nebolo poriesene tak by boli naopak cerveny by bol v druhom okne sedy a naopak 
		int pom = this->hrac->getPolohaY();
		this->hrac->setInitY(this->protihrac->getPolohaY());
		this->protihrac->setInitY(pom);
		this->protihrac->setFarbuHraca(100, 100, 100);
	}
	else {
		this->hrac->setFarbuHraca(100, 100, 100); //nastavenie aby boli v oboch oknach rovnake farby pre urovni hraca
	}

	for (int i = 0; i < prekazkyVriadku; i++) {
		for (int j = 0; j < prekazkyVStlpci; j++) {
			this->prekazka[i][j] = new Prekazka(i, j);
		}
	}
}

void HraciaPlocha::vytvorNovuHru(int dalsiLevel)
{
	lopta = new Lopta();
	hrac = new Hrac();
	protihrac = new Hrac();
	protihrac->setInitY();
	hrac->zmenSirku(dalsiLevel);
	protihrac->zmenSirku(dalsiLevel);
	//lopta->zrychliLopticku(dalsiLevel);
	if (this->isServer) { // ak je sserver tak vymeni kvazy hracov aby sa to zobrazovalo srpavne ak by toto nebolo poriesene tak by boli naopak cerveny by bol v druhom okne sedy a naopak 
		int pom = this->hrac->getPolohaY();
		this->hrac->setInitY(this->protihrac->getPolohaY());
		this->protihrac->setInitY(pom);
		this->protihrac->setFarbuHraca(100, 100, 100);
	}
	else {
		this->hrac->setFarbuHraca(100, 100, 100); //nastavenie aby boli v oboch oknach rovnake farby pre urovni hraca
	}

	for (int i = 0; i < prekazkyVriadku; i++) {
		for (int j = 0; j < prekazkyVStlpci; j++) {
			this->prekazka[i][j] = new Prekazka(i, j);
		}
	}
}

bool HraciaPlocha::kontrolaPrienikov(Lopta* lopta, Hrac* hrac) //kontrola ci sa lopta odrazi od hraca
{
	SDL_Rect R; //vytvorenie premenej na stacku nej ju potom zahodi 
	return SDL_IntersectRect(&lopta->getSeba(), &hrac->getSeba(), &R);
}

int __cdecl HraciaPlocha::client()
{
	WSADATA wsaData;
	int iResult;
	struct addrinfo* result = NULL, *ptr = NULL, hints;
	const char* sendBuff = "testovacia sprava\n";
	char prijate[DEFAULT_BUFLEN];

	int recvbuflen = DEFAULT_BUFLEN;

	//1. krok - inicializacia winsock DDL
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		printf("WSA inicializacia sa nepodarila, skoncila s chybou: %d \n", iResult);
		WSACleanup();
		return 1;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	iResult = getaddrinfo("127.0.0.1", DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		printf("Zlyhanie getaddrinfos chybou: %d\n", iResult);
		WSACleanup();
		return 1;
	}

	// Attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

		// Create a SOCKET for connecting to server
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (ConnectSocket == INVALID_SOCKET) {
			printf("Zlyhanie soketu s chybou: %ld\n", WSAGetLastError());
			WSACleanup();
			return 1;
		}

		// Pripojenie na server.
		iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	if (ConnectSocket == INVALID_SOCKET) {
		printf("Problem s pripojenim k serveru!\n");
		WSACleanup();
		return 1;
	}

	std::thread prijimanieVlakno(&HraciaPlocha::vlaknoPrijimanie, this);
	odchytanieUdalostiLogFun();

	prijimanieVlakno.join();


	iResult = shutdown(ConnectSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("Ukoncenie s chybou: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}
	closesocket(ConnectSocket);
	WSACleanup();

	return 0;
}

int __cdecl HraciaPlocha::server()
{
	WSADATA wsaData;
	int iResult;

	SOCKET ListenSocket = INVALID_SOCKET;

	struct addrinfo* result = NULL;
	struct addrinfo hints;

	int iSendResult;
	char prijate[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("Zlyhalo WSAStartup s chybou: %d\n", iResult);
		return 1;
	}

	//Vynulujeme a zinicializujeme sieovú adresu
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	//Nacitame sieovú adresu a port
	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		printf("Zlyhalo getaddrinfo s chybou: %d\n", iResult);
		WSACleanup();
		return 1;
	}

	// Vytvori socket na pripojenie ku serveru
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ListenSocket == INVALID_SOCKET) {
		printf("Zlyhal socket s chybou: %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	// Nastavi TCP socket na listening
	iResult = ::bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		printf("Zlyhalo bind s chybou: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	freeaddrinfo(result);

	iResult = listen(ListenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR) {
		printf("Zlyhalo listen s chybou: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	// Prijatie klientskeho socketu
	ConnectSocket = accept(ListenSocket, NULL, NULL);
	if (ConnectSocket == INVALID_SOCKET) {
		printf("Zlyhalo accept s chybou: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	// Uz nieje potreba serveroveho socketu
	closesocket(ListenSocket);


	/////////////////CELE TO BUDE TU
	std::thread prijimanieVlakno(&HraciaPlocha::vlaknoPrijimanie, this);
	odchytanieUdalostiLogFun();

	prijimanieVlakno.join();

	iResult = shutdown(ConnectSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("Ukoncenie s chybou: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}
	closesocket(ConnectSocket);
	WSACleanup();

	return 0;
}

void HraciaPlocha::odchytanieUdalostiLogFun()
{
	//bool inicHrac = false;
	vytvorNovuHru();
	while (!this->koniec) {
		//odoslanie(nicNeposiela, nicNeposiela, nicNeposiela, nicNeposiela, nicNeposiela, nicNeposiela, this->vyberAkcie);
		/*
		if ((this->vyberAkcie == 0 && this->level == 1 && !this->jeZobrazeneMenu)|| prehraOdos || vyhraOdos) { //ak je to prvy level a zaroven nieje vykreslene hlavne menu, ak nie tak vykresli uvodnu obrazovku START/KONIEC
			this->vykresliMenu(); //vykresli menu na zaklade toho ci je server alebo klient pooriesene v konkretnej metode
			this->jeZobrazeneMenu = true; // zmeni atribut na true aby sa stale neprekreslovalo v cykle

		}*/
		if (!this->jeZobrazeneMenu) { //ak je to prvy level a zaroven nieje vykreslene hlavne menu, ak nie tak vykresli uvodnu obrazovku START/KONIEC
			this->vykresliMenu(); //vykresli menu na zaklade toho ci je server alebo klient pooriesene v konkretnej metode
			this->jeZobrazeneMenu = true; // zmeni atribut na true aby sa stale neprekreslovalo v cykle 
			if (isServer) {
				odoslanie(nicNeposiela, nicNeposiela, nicNeposiela, nicNeposiela, nicNeposiela, nicNeposiela, this->vyberAkcie, nicNeposiela, nicNeposiela);
			}
		}
		else if (this->vyberAkcie == 1) { //toto znamena ze na servery si hrac vybral START


			vycistiOkno(); //vycistenie okna 
			Sleep(1); // synchronizacia rychlosti clienta a serveru
			if (this->pauza == false) { //kontrola ci si pozastavil hrac hru medzernikom plati iba pri servery, klient si hu nemoze pozastavit
				if (isServer) { //logika lopticky, vyratava server a len odosiela klientovu
					if (this->kontrolaPrienikov(lopta, hrac) || this->kontrolaPrienikov(lopta, protihrac)) { // ci sa lopta odrazila od hraca/ protihraca
						lopta->uder(); //ak ano vytvori uder kde prenasobi deltu Y -1 alebo deltuX na zaklade od coho sa odrazila, hrac/stena
					}
					else {
						lopta->krok(); // posun lopticky
					}
					odoslanie(lopta->getPolohaX(), lopta->getPolohaY(), nicNeposiela, nicNeposiela, nicNeposiela, nicNeposiela, nicNeposiela, nicNeposiela, nicNeposiela);
				}
			}

			bool trafilPrekazku = false;
			for (int i = 0; i < prekazkyVriadku; i++) { //prechadzanie dvojrozmerneho pola prekazok
				for (int j = 0; j < prekazkyVStlpci; j++) {

					if (this->prekazka[i][j]->siZobreazena() && this->jeZasahPrekazky(prekazka[i][j], this->lopta)) { //ak je lopta zobrazena tak zistuje ci sa trafila do prekazky
						bool smer = this->lopta->getPolohaX() > (this->prekazka[i][j]->getPolohaX() - this->lopta->getSirku() + 1) &&
							this->lopta->getPolohaX() < (this->prekazka[i][j]->getPolohaX() + this->prekazka[i][j]->getSirku() - 1);
						this->lopta->uderPrekazky(smer); //odraz od prekazky aby zmenila smer 
						trafilPrekazku = true;

						if (this->isServer && trafilPrekazku) {
							this->skryTrafenuPrekazku(i, j);
							this->skoreCelk += Constants::SKORE; //prirata skore	

							this->odoslanie(lopta->getPolohaX(), lopta->getPolohaY(), nicNeposiela, i, j, this->skoreCelk, nicNeposiela, nicNeposiela, nicNeposiela);
							//odosle poziciu lopticky a odosle polohu prekazky ktoru zmeni na "zhasnutu"					
						}
						trafilPrekazku = false;
						break;
					}
					this->vykresliPrekazku(i, j); // tto len nastavi do SDL renderu ze tato sa vykresli 
				}
			}

			//this->skoreCelk += this->skore; //prirata do celkoveho skore
			this->vykresliHra(); //vykreli hracov a loptu a vsetky prekazky naraz!!

			if (this->isServer && this->jePrehra(this->lopta)) {

				this->skore = 0;
				this->prehraOdos = true;
				this->vyberAkcie = 0; //vykresli sa uvodne menu 

				odoslanie(nicNeposiela, nicNeposiela, nicNeposiela, nicNeposiela, nicNeposiela, this->skoreCelk, this->vyberAkcie, 1, nicNeposiela);
				this->vypisSkore(this->skoreCelk); //vypisanie skore, oneskorenie vytvorene cez sdl delay	
				this->prehra = false;

				this->level = 1;
				this->skoreCelk = 0;
			}
			else if (this->isServer && this->jeVyhra(this->skoreCelk)) {
				int pom = this->skoreCelk;
				this->vyhraOdos = true;
				this->skoreCelk = pom;
				this->vyberAkcie = 1;
				this->vyhra = false;
				this->level += 1; //prida level, ktory aj vypise
				if (this->level > Constants::POCET_LEVELOV) {
					this->vyberAkcie = 0;
					this->jeZobrazeneMenu = false;
					this->skoreCelk = 0;
					this->level = 1;
					odoslanie(nicNeposiela, nicNeposiela, nicNeposiela, nicNeposiela, nicNeposiela, this->skoreCelk, this->vyberAkcie, 1, nicNeposiela);
					this->vypisSkore(this->skoreCelk); //vypisanie skore, oneskorenie vytvorene cez sdl delay
				}
				else {
					odoslanie(nicNeposiela, nicNeposiela, nicNeposiela, nicNeposiela, nicNeposiela, this->skoreCelk, 0, nicNeposiela, 1);
					this->vypisSkore(this->skoreCelk); //vypisanie skore, oneskorenie vytvorene cez sdl delay
				}

			}

			if (this->prehraOdos || this->vyhraOdos) {
				//sem musim vymazat staru lopta a hraca a prekazky, vypise skore a moze sa spustit nova hra
				this->vymazStaruHru();
				if (this->level <= Constants::POCET_LEVELOV) {
					//vytvorit noveho hraca loptu a prekazky abymohli hrat od znova
					this->vytvorNovuHru(this->level);
				}
				this->jeZobrazeneMenu = false;
				this->prehraOdos = false;
				this->vyhraOdos = false;
			}

		}
		while (SDL_PollEvent(udalost)) //odchytavanie udalosti - klavesy a mys 
		{
			if (udalost->type == SDL_QUIT && isServer) {
				this->koniec = true;
				this->odoslanie(nicNeposiela, nicNeposiela, nicNeposiela, nicNeposiela, nicNeposiela, nicNeposiela, nicNeposiela, nicNeposiela, nicNeposiela);
			}
			else if (udalost->type == SDL_KEYDOWN) {
				switch (udalost->key.keysym.sym) {
				case SDLK_RIGHT:
					if (!this->pauza) {
						this->hrac->setPolohaX(Constants::POSUN_HRACA_KLADNE); //sem bz malo nastavit polohu x+4
						this->odoslanie(nicNeposiela, nicNeposiela, Constants::POSUN_HRACA_KLADNE, nicNeposiela, nicNeposiela, nicNeposiela, nicNeposiela, nicNeposiela, nicNeposiela);
					}
					break;
				case SDLK_LEFT:
					if (!this->pauza) {
						this->hrac->setPolohaX(Constants::POSUN_HRACA_ZAPORNE); // sem bz malo nastavit polohu x-4 
						this->odoslanie(nicNeposiela, nicNeposiela, Constants::POSUN_HRACA_ZAPORNE, nicNeposiela, nicNeposiela, nicNeposiela, nicNeposiela, nicNeposiela, nicNeposiela);
					}
					break;

				case SDLK_SPACE:
					this->pauza = this->pauza ? false : true;
					break;
				}

			}
			else if (udalost->button.clicks == SDL_BUTTON_LEFT && isServer) { //pre mys a pre prehladnost vytvorene premenne dalo sa to aj bez nich
				int XLStart = this->menu->getStart().x;
				int YLStart = this->menu->getStart().y;
				int sirka = this->menu->getSirka();
				int vyska = this->menu->getVyska();

				int XLKoniec = this->menu->getKoniec().x;
				int YLKoniec = this->menu->getKoniec().y;

				int motionX = udalost->motion.x;
				int motionY = udalost->motion.y;

				if (motionX > XLStart &&
					(motionX < XLStart + sirka) &&
					(motionY > YLStart - vyska) &&
					(motionY < YLStart - vyska + sirka)
					) {
					this->vyberAkcie = 1;
					this->odoslanie(nicNeposiela, nicNeposiela, nicNeposiela, nicNeposiela, nicNeposiela, nicNeposiela, this->vyberAkcie, nicNeposiela, nicNeposiela);

				}
				else if (motionX > XLKoniec &&
					(motionX < XLKoniec + sirka) &&
					(motionY > YLKoniec - vyska) &&
					(motionY < YLKoniec - vyska + sirka)
					) {
					this->koniec = true;
					this->odoslanie(nicNeposiela, nicNeposiela, nicNeposiela, nicNeposiela, nicNeposiela, nicNeposiela, nicNeposiela, nicNeposiela, nicNeposiela);
				}
				//odoslanie ze ci sa stlacil koniec alebo start .. toto som spravila na zaklade vyberAkcie a koniec riesi sa v metode odoslanie
			}
		}

	}
}
void HraciaPlocha::odoslanie(int loptaX, int loptaY, int hracX, int prekPoleI, int prekPoleJ, int skore, int vybAkc, int parPrehra, int parVyhra)
{
	string sprava; //tato sprava sa posiela

	stringstream sHX;
	sHX << hracX;
	string strHX = sHX.str();

	stringstream sLX;
	sLX << loptaX;
	string strLX = sLX.str();

	stringstream sLY;
	sLY << loptaY;
	string strLY = sLY.str();

	stringstream sPPI;
	sPPI << prekPoleI;
	string strPPI = sPPI.str();

	stringstream sPPJ;
	sPPJ << prekPoleJ;
	string strPPJ = sPPJ.str();

	stringstream sSkore;
	sSkore << skore;
	string strSkore = sSkore.str();

	stringstream sVybAkc;
	sVybAkc << vybAkc;
	string strVybAkc = sVybAkc.str();

	stringstream sPrehra;
	sPrehra << parPrehra;
	string strPrehra = sPrehra.str();

	stringstream sVyhra;
	sVyhra << parVyhra;
	string strVyhra = sVyhra.str();

	int koniecOdos = this->koniec ? 1 : nicNeposiela;
	stringstream ssEND; ssEND << koniecOdos; string strEND = ssEND.str();

	sprava += strHX + " " + strLX + " " + strLY + " " + strPPI + " " + strPPJ + " " + strSkore + " " + strVybAkc + " " + strPrehra + " " + strVyhra + " " + strEND;

	int iResult = ::send(ConnectSocket, sprava.c_str(), 1000, 0);

	if (iResult == SOCKET_ERROR) {
		printf("Odosielanie skoncilo s chybou: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return;
	}
}

void HraciaPlocha::vlaknoPrijimanie()
{
	char prijate[1000];
	while (!this->koniec) {
		int iResult = recv(ConnectSocket, prijate, 1000, 0);
		if (iResult == SOCKET_ERROR) {
			printf("Ukoncenie s chybou: %d\n", WSAGetLastError());
			closesocket(ConnectSocket);
			WSACleanup();
			return;
		}

		istringstream buffer(prijate);
		istream_iterator<string> beg(buffer), end;
		vector<string> pole(beg, end);

		this->mut.lock();
		if (pole.size() == 10) {
			if (atoi(pole[0].c_str()) != nicNeposiela) {
				this->protihrac->setPolohaX(atoi(pole[0].c_str()));
			}
			if (atoi(pole[1].c_str()) != nicNeposiela && atoi(pole[2].c_str()) != nicNeposiela) {
				this->lopta->setPolohaX(atoi(pole[1].c_str()));
				this->lopta->setPolohaY(atoi(pole[2].c_str()));
			}

			if (atoi(pole[3].c_str()) != nicNeposiela || atoi(pole[4].c_str()) != nicNeposiela) {
				this->skryTrafenuPrekazku(atoi(pole[3].c_str()), atoi(pole[4].c_str()));
			}

			if (atoi(pole[5].c_str()) != nicNeposiela) {
				this->skoreCelk = atoi(pole[4].c_str());
			}

			if (atoi(pole[6].c_str()) != nicNeposiela) {
				this->vyberAkcie = atoi(pole[6].c_str());
			}

			if (atoi(pole[7].c_str()) != nicNeposiela) {
				this->prehraOdos = true;
				this->level = 1;
			}
			if (atoi(pole[8].c_str()) != nicNeposiela) {
				this->vyhraOdos = true;
				this->level += 1;
			}



			if (atoi(pole[9].c_str()) != nicNeposiela) {
				this->koniec = true;
			}
		}
		this->mut.unlock();
	}
}

bool HraciaPlocha::jePrehra(Lopta* parLopta)
{
	if ((parLopta->getPolohaY() + parLopta->getDeltaY()) >= Constants::VYSKA - parLopta->getVysku()) {
		return this->prehra = true;
	}
	return this->prehra;
}

bool HraciaPlocha::jeVyhra(int parSkore)
{
	if (parSkore / 10 == prekazkyVriadku * prekazkyVStlpci * this->level) {
		return true;
	}
	return false;
}

void HraciaPlocha::vykresliMenu()
{
	vycistiOkno();

	TTF_Init();

	if (TTF_Init() == -1) {
		printf("TTF_Init: %s\n", TTF_GetError());
		exit(2);
	}
	TTF_Font* fontTextu = TTF_OpenFont("verdana.ttf", 100);
	//TTF_Font* fontTextuClient = TTF_OpenFont("verdana.ttf", 60);
	SDL_Color farbaTextu = { 0, 0, 0, 255 };

	if (isServer) {

		SDL_Surface* textSurfaceStart = TTF_RenderText_Solid(fontTextu, "Start", farbaTextu);
		SDL_Texture* textStart = SDL_CreateTextureFromSurface(this->renderer, textSurfaceStart);
		SDL_FreeSurface(textSurfaceStart);
		//SDL_SetRenderDrawColor(this->renderer, 0, 255, 0, 255);
		//SDL_RenderFillRect(this->renderer, &this->menu->getStart());
		SDL_RenderCopy(this->renderer, textStart, NULL, &this->menu->getStart());

		SDL_Surface* textSurfaceKoniec = TTF_RenderText_Solid(fontTextu, "Koniec", farbaTextu);
		SDL_Texture* textKoniec = SDL_CreateTextureFromSurface(this->renderer, textSurfaceKoniec);
		SDL_FreeSurface(textSurfaceKoniec);
		//SDL_SetRenderDrawColor(this->renderer, 0, 255, 0, 255);
		//SDL_RenderFillRect(this->renderer, &this->menu->getKoniec());
		SDL_RenderCopy(this->renderer, textKoniec, NULL, &this->menu->getKoniec());


		SDL_RenderPresent(this->renderer);

		SDL_DestroyTexture(textStart);
		SDL_DestroyTexture(textKoniec);
	}
	else {
		SDL_Surface* textSurfaceClient = TTF_RenderText_Solid(fontTextu, "Caka na server", farbaTextu);
		SDL_Texture* textClient = SDL_CreateTextureFromSurface(this->renderer, textSurfaceClient);
		SDL_FreeSurface(textSurfaceClient);
		//SDL_SetRenderDrawColor(this->renderer, 0, 255, 0, 255);

		//SDL_RenderFillRect(this->renderer, &this->menu->getStart());
		SDL_RenderCopy(this->renderer, textClient, NULL, &this->menu->getLevel());

		SDL_RenderPresent(this->renderer);

		SDL_DestroyTexture(textClient);
	}

	TTF_Quit();

}

bool HraciaPlocha::jeZasahPrekazky(Prekazka* prekazka, Lopta *lopta)
{
	SDL_Rect R;
	return SDL_IntersectRect(&prekazka->getSeba(), &lopta->getSeba(), &R);
}

void HraciaPlocha::vycistiOkno()
{
	SDL_SetRenderDrawColor(this->renderer, 255, 255, 255, 255);
	SDL_RenderClear(this->renderer);
}

void HraciaPlocha::vykresliHra() //ttooto vykresli vsetko hracov, loptu, prekazky
{
	SDL_SetRenderDrawColor(this->renderer, this->hrac->getFarba(0), this->hrac->getFarba(1), this->hrac->getFarba(2), 255);
	SDL_RenderFillRect(this->renderer, &hrac->getSeba());

	SDL_SetRenderDrawColor(this->renderer, 0, 0, 255, 255);
	SDL_RenderFillRect(this->renderer, &lopta->getSeba());

	SDL_SetRenderDrawColor(this->renderer, this->protihrac->getFarba(0), this->protihrac->getFarba(1), this->protihrac->getFarba(2), 255);
	SDL_RenderFillRect(this->renderer, &protihrac->getSeba());

	prekresli(); //vykresli vsetko
}
void HraciaPlocha::vykresliPrekazku(int i, int j)
{
	if (prekazka[i][j]->siZobreazena()) {
		SDL_SetRenderDrawColor(this->renderer, 0, 255, 0, 255);
		SDL_RenderFillRect(this->renderer, &prekazka[i][j]->getSeba());
	}
}


void HraciaPlocha::prekresli()
{
	SDL_RenderPresent(this->renderer);
	//	vycistiOkno();
}

void HraciaPlocha::skryTrafenuPrekazku(int i, int j)
{
	prekazka[i][j]->setZobrazena(false);
	SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 255);
	SDL_RenderFillRect(this->renderer, &prekazka[i][j]->getSeba());
}


void HraciaPlocha::vypisSkore(int parSkore)
{

	vycistiOkno();
	std::string textSkore = "Skore: " + std::to_string(parSkore);
	std::string textLevel;
	if (this->prehraOdos) {
		textLevel = "Skoncils v leveli: " + std::to_string(level);
	}
	else if (this->level <= Constants::POCET_LEVELOV) {
		textLevel = "Dalsi level: " + std::to_string(level);
	}
	else if (this->level > Constants::POCET_LEVELOV) {
		textLevel = "Dali ste to :D ";
	}

	TTF_Init();

	if (TTF_Init() == -1) {
		printf("TTF_Init: %s\n", TTF_GetError());
		exit(2);
	}

	TTF_Font* fontTextu = TTF_OpenFont("verdana.ttf", 100);
	SDL_Color farbaTextu = { 0, 0, 0, 255 };

	SDL_Surface* textSurfaceSkore = TTF_RenderText_Solid(fontTextu, textSkore.c_str(), farbaTextu);
	SDL_Texture* textSkoreTexture = SDL_CreateTextureFromSurface(this->renderer, textSurfaceSkore);
	SDL_FreeSurface(textSurfaceSkore);
	//SDL_SetRenderDrawColor(this->renderer, 0, 255, 0, 255);
	//SDL_RenderFillRect(this->renderer, &this->menu->getSkore());
	SDL_RenderCopy(this->renderer, textSkoreTexture, NULL, &this->menu->getSkore());

	SDL_Surface* textSurfaceLevel = TTF_RenderText_Solid(fontTextu, textLevel.c_str(), farbaTextu);
	SDL_Texture* textLevelTexture = SDL_CreateTextureFromSurface(this->renderer, textSurfaceLevel);
	SDL_FreeSurface(textSurfaceLevel);
	//SDL_SetRenderDrawColor(this->renderer, 0, 255, 0, 255);
	//SDL_RenderFillRect(this->renderer, &this->menu->getStart()); //vykresli to do konca obdlznik :D nechce sa mi vytvarat dalsi :D 
	SDL_RenderCopy(this->renderer, textLevelTexture, NULL, &this->menu->getLevel());

	SDL_RenderPresent(this->renderer);
	SDL_Delay(3000);
	SDL_DestroyTexture(textSkoreTexture);
	SDL_DestroyTexture(textLevelTexture);
	TTF_Quit();

}


