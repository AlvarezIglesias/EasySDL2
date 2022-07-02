# EasySDL2
EasySDL2 is a single-file newbie-oriented wrapper for SDL2. 

A lot of new programming students start with c++ and/or object oriented programming. Most of the time, all they do is make console programs, which is not very visual and usually boring. Most other graphics libraries and wrappers require the knoledge of pointers or set-ups that are not intended for newcomers. EasySDL2 hides set-up, configuration, and complex concepts susch as renderers and surfacers from the programmer.

This repo is still in active development as it is lacking some basic features, but it is functional and al contributions are welcome if they follow the same newbie aproach.

Some components, such as textures, are managed through smart pointers, so c++11 is required.

## How to use
### Windows and Visual studio
In your project, open nugget package manger and install SDL2, SDL_image and SDL_mixer. 
Include this hpp in your files
On top of your .cpp file, add: #include "EasySDL2.hpp"


## Basics
```
#include "EasySDL2.hpp"
using namespace std;

int main(int argc, char* argv[]) {

	EasySDL2 sdl(500, 500);                 //initialization
	Texture t = sdl.loadTexture("h.jpg");   //load texture
	sdl.drawTexture(t, 0, 0, 500, 500); 
	sdl.drawFrame();                        //show next frame
	sdl.waitUntil("Q");                     //show same frame until Q is pressed
	return 0;

}
```
