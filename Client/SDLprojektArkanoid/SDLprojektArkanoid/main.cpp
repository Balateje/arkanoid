#include <SDL.h>
#include "HraciaPlocha.h"


int main(int argc, char* argv[])
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	HraciaPlocha *hracia = new HraciaPlocha();
	hracia->spusti(false);
	
	delete hracia;
	return 0;
}