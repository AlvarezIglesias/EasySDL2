#include "EasySDL2.hpp"
using namespace std;

int main(int argc, char* argv[]) {

	EasySDL2 sdl(500, 500);
	Texture t = sdl.loadTexture("h.jpg");
	sdl.drawTexture(0, 0, 500, 500, t);
	sdl.drawFrame();
	sdl.waitUntil("Q");
	return 0;

}