#include "EasySDL2.hpp"
using namespace std;

int main(int argc, char* argv[]) {

	EasySDL2 sdl(500, 500);
	while (true)
	{
		Texture t = sdl.loadTexture("cat.jpg");
		Texture tt = t;
		sdl.drawTexture(0, 0, 500, 500, tt);
		sdl.drawFrame();

	}

	return 0;

}