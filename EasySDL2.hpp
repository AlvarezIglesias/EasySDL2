#pragma once
#ifndef EASYSDL2_H_
#define EASYSDL2_H_

#include "SDL.h"
#include "SDL_image.h"
#include <iostream>
#include <unordered_map>
#include <iostream>

struct Color
{
	int r, g, b, a;
};

struct Point
{
	int x, y;
};

struct Texture
{
	SDL_Texture* texture;
	bool flipX;
	bool flipY;

private:
	//~Texture() { SDL_DestroyTexture(texture); }; //might cause confusion
};

class EasySDL2
{
private:

	SDL_Window* window;
	SDL_Surface* surface;
	SDL_Renderer* renderer;


	void init();
	void init(int width, int height);
	void init(int width, int height, std::string name);

	void detect();
	void updateKeyMaps();
	void _drawCircle(SDL_Renderer* renderer, int centreX, int centreY, int radius);
	void _drawCircleFilled(SDL_Renderer* renderer, int centreX, int centreY, int radius);


	static std::unordered_map<std::string, bool> KEYS;
	static std::unordered_map<std::string, bool> lastKEYS;

	static bool mouseButtons[3];
	static bool lastMouseButtons[3];
	


public:

	EasySDL2() { init(); };
	EasySDL2(int width, int height) { init(width, height); };
	EasySDL2(int width, int height, std::string name) { init(width, height, name); };

	~EasySDL2() {
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_FreeSurface(surface);
	};

	void drawLine(int x1, int y1, int x2, int y2);
	void drawLine(int x1, int y1, int x2, int y2, Color color);

	void drawRect(int x, int y, int width, int height, Color color);
	void drawRect(int x, int y, int width, int height, float xPivot, float yPivot, Color color);

	void drawRectFilled(int x, int y, int width, int height, Color color);
	void drawRectFilled(int x, int y, int width, int height, float xPivot, float yPivot, Color color);

	void drawCircle(int x, int y, int radius, Color color);
	void drawCircleFilled(int x, int y, int radius, Color color);

	void drawTexture(int x, int y, int width, int height, Texture texture);
	void drawTexture(int x, int y, int width, int height, int angle, Texture texture);
	void drawTexture(int x, int y, int width, int height, int xPivot, int yPivot, Texture texture);
	void drawTexture(int x, int y, int width, int height, int xPivot, int yPivot, float angle, Texture texture);

	Texture loadTexture(std::string path);

	bool checkKeyDown(std::string keyName);
	bool checkKey(std::string keyName);
	bool checkKeyUp(std::string keyName);

	bool mouseButtonDown(int buttonIndex);
	bool mouseButton(int buttonIndex);
	bool mouseButtonUp(int buttonIndex);

	void showUntil(std::string key);
	void showUntil(int key);

	void drawFrame();

	static Uint64 time;
	static double deltaTime;

	int xMouse;
	int yMouse;
	int xWindowPos;
	int yWindowPos;
	static int xMouseAbsolute;
	static int yMouseAbsolute;
	static int screenWidth;
	static int screenHeight;

	static bool exit;

	static const Color RED;
	static const Color GREEN;
	static const Color BLUE;
	static const Color BLACK;
	static const Color WHITE;

};

const Color EasySDL2::RED = { 255,0,0,255 };
const Color EasySDL2::GREEN = { 0,255,0,255 };
const Color EasySDL2::BLUE = { 0,0,255,255 };
const Color EasySDL2::WHITE = { 255,255,255,255 };
const Color EasySDL2::BLACK = { 0,0,0,255 };

std::unordered_map<std::string, bool> EasySDL2::KEYS;
std::unordered_map<std::string, bool> EasySDL2::lastKEYS;

bool EasySDL2::mouseButtons[3] = { false,false,false };
bool EasySDL2::lastMouseButtons[3] = { false,false,false };

Uint64 EasySDL2::time;
double EasySDL2::deltaTime;

int EasySDL2::xMouseAbsolute;
int EasySDL2::yMouseAbsolute;
int EasySDL2::screenWidth;
int EasySDL2::screenHeight;

bool EasySDL2::exit = false;




void EasySDL2::init()
{
	init(600, 600);
}

void EasySDL2::init(int width, int height)
{
	init(width, height, "easySDLwindow");
}

void EasySDL2::init(int width, int height, std::string name)
{

	window = SDL_CreateWindow(
		name.c_str(),
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		width, height,
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_ALWAYS_ON_TOP
	);
	renderer = SDL_CreateRenderer(window, -1, 0);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);

	surface = SDL_GetWindowSurface(window);

	time = SDL_GetPerformanceCounter();

	drawFrame();

}

void EasySDL2::drawLine(int x1, int y1, int x2, int y2) {
	drawLine(x1, y1, x2, y2, BLACK);
}
void EasySDL2::drawLine(int x1, int y1, int x2, int y2, Color color) {
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}

void EasySDL2::drawRect(int x, int y, int width, int height, Color color)
{
	drawRect(x, y, width, height, .0f, .0f, color);
}

void EasySDL2::drawRect(int x, int y, int width, int height, float xPivot, float yPivot, Color color)
{
	SDL_Rect dest = { x - xPivot, y - yPivot,width ,height };
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_RenderDrawRect(renderer, &dest);
}

void EasySDL2::drawRectFilled(int x, int y, int width, int height, Color color)
{
	drawRectFilled(x, y, width, height, .0f, .0f, color);
}

void EasySDL2::drawRectFilled(int x, int y, int width, int height, float xPivot, float yPivot, Color color)
{
	SDL_Rect dest = { x - xPivot, y - yPivot,width ,height };
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_RenderFillRect(renderer, &dest);
}

void EasySDL2::drawCircle(int x, int y, int radius, Color color) {

	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	_drawCircle(renderer, x, y, radius);
}

void EasySDL2::drawCircleFilled(int x, int y, int radius, Color color)
{
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	_drawCircleFilled(renderer, x, y, radius);
}

void EasySDL2::drawTexture(int x, int y, int width, int height, Texture texture)
{
	drawTexture(x, y, width, height, .0f, .0f, texture);
}

void EasySDL2::drawTexture(int x, int y, int width, int height, int angle, Texture texture)
{
	drawTexture(x, y, width, height, 0, 0, angle, texture);
}

void EasySDL2::drawTexture(int x, int y, int width, int height, int xPivot, int yPivot, Texture texture)
{
	drawTexture(x, y, width, height, xPivot, yPivot, 0, texture);
}

void EasySDL2::drawTexture(int x, int y, int width, int height, int xPivot, int yPivot, float angle, Texture texture)
{
	SDL_Rect dest = { x,y,width,height };
	SDL_Point pivot = { xPivot, yPivot };
	SDL_RendererFlip flip = static_cast<SDL_RendererFlip>((
		texture.flipX ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE) |
		(texture.flipY ? SDL_FLIP_VERTICAL : SDL_FLIP_NONE
			));

	SDL_RenderCopyEx(renderer, texture.texture, NULL, &dest, angle, &pivot, flip);
}

Texture EasySDL2::loadTexture(std::string path)
{
	SDL_Surface* imgSurface = IMG_Load(path.c_str());
	SDL_Surface* optimizedImgSurface = NULL;

	optimizedImgSurface = SDL_ConvertSurface(imgSurface, surface->format, 0);
	SDL_FreeSurface(imgSurface);

	if (optimizedImgSurface == NULL) std::cout << "no se ha encontrado el archivo" << std::endl;

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, optimizedImgSurface);

	return { texture };
}

void EasySDL2::drawFrame()
{
	SDL_RenderPresent(renderer);

	SDL_Delay(1);

	updateKeyMaps();
	detect();

	Uint64 lastTime = time;
	time = SDL_GetPerformanceCounter() / 1000.f;
	deltaTime = (time - lastTime) / 1000.f;

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);
}

//https://wiki.libsdl.org/SDL_Keycode
bool EasySDL2::checkKeyDown(std::string keyName)
{
	return EasySDL2::KEYS[keyName] && (!EasySDL2::lastKEYS[keyName]);
}

//https://wiki.libsdl.org/SDL_Keycode
bool EasySDL2::checkKey(std::string keyName)
{
	return EasySDL2::KEYS[keyName];
}

//https://wiki.libsdl.org/SDL_Keycode
bool EasySDL2::checkKeyUp(std::string keyName)
{
	return (!EasySDL2::KEYS[keyName]) && EasySDL2::lastKEYS[keyName];
}

bool EasySDL2::mouseButtonDown(int buttonIndex) {
	return EasySDL2::mouseButtons[buttonIndex] && (!EasySDL2::lastMouseButtons[buttonIndex]);
}

bool EasySDL2::mouseButton(int buttonIndex) {
	return EasySDL2::mouseButtons[buttonIndex];
}

bool EasySDL2::mouseButtonUp(int buttonIndex) {
	return (!EasySDL2::mouseButtons[buttonIndex]) && EasySDL2::lastMouseButtons[buttonIndex];
}

void EasySDL2::showUntil(std::string keyName)
{
	while (!checkKeyDown(keyName)) {
		detect();
		SDL_RenderPresent(renderer);
		SDL_Delay(1);
	}
}

void EasySDL2::showUntil(int mouseIndex)
{
	while (!mouseButtonDown(mouseIndex)) {
		detect();
		SDL_RenderPresent(renderer);
		SDL_Delay(1);
	}
}

void EasySDL2::updateKeyMaps() {

	for (int i = 0; i < 3; i++)
	{
		EasySDL2::lastMouseButtons[i] = EasySDL2::mouseButtons[i];
	}

	for (auto x : EasySDL2::KEYS)
		EasySDL2::lastKEYS[x.first] = x.second;
}

void EasySDL2::detect() {
	SDL_Event event;

	SDL_GetGlobalMouseState(&xMouseAbsolute, &yMouseAbsolute);
	SDL_GetWindowPosition(window, &xWindowPos, &yWindowPos);
	//SDL_GetMouseState(&xMouse, &yMouse);

	xMouse = xMouseAbsolute - xWindowPos;
	yMouse = yMouseAbsolute - yWindowPos;

	while (SDL_PollEvent(&event)) {
		// check for messages
		//SDLK_LEFT
		switch (event.type) {
			// exit if the window is closed
		case SDL_QUIT:
			exit = true;
			break;
			// check for keypresses
		case SDL_KEYDOWN:
			EasySDL2::KEYS[SDL_GetKeyName(event.key.keysym.sym)] = true;
			break;
		case SDL_KEYUP:
			EasySDL2::KEYS[SDL_GetKeyName(event.key.keysym.sym)] = false;
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (event.button.button == SDL_BUTTON_LEFT) EasySDL2::mouseButtons[0] = true;
			if (event.button.button == SDL_BUTTON_MIDDLE) EasySDL2::mouseButtons[1] = true;
			if (event.button.button == SDL_BUTTON_RIGHT) EasySDL2::mouseButtons[2] = true;
			break;
		case SDL_MOUSEBUTTONUP:
			if (event.button.button == SDL_BUTTON_LEFT) EasySDL2::mouseButtons[0] = false;
			if (event.button.button == SDL_BUTTON_MIDDLE) EasySDL2::mouseButtons[1] = false;
			if (event.button.button == SDL_BUTTON_RIGHT) EasySDL2::mouseButtons[2] = false;
			break;
		default:
			break;
		}
	}


}

void EasySDL2::_drawCircle(SDL_Renderer* renderer, int centreX, int centreY, int radius)
{
	const int32_t diameter = (radius * 2);

	int32_t x = (radius - 1);
	int32_t y = 0;
	int32_t tx = 1;
	int32_t ty = 1;
	int32_t error = (tx - diameter);

	while (x >= y)
	{
		//  Each of the following renders an octant of the circle
		SDL_RenderDrawPoint(renderer, centreX + x, centreY - y);
		SDL_RenderDrawPoint(renderer, centreX + x, centreY + y);
		SDL_RenderDrawPoint(renderer, centreX - x, centreY - y);
		SDL_RenderDrawPoint(renderer, centreX - x, centreY + y);
		SDL_RenderDrawPoint(renderer, centreX + y, centreY - x);
		SDL_RenderDrawPoint(renderer, centreX + y, centreY + x);
		SDL_RenderDrawPoint(renderer, centreX - y, centreY - x);
		SDL_RenderDrawPoint(renderer, centreX - y, centreY + x);

		if (error <= 0)
		{
			++y;
			error += ty;
			ty += 2;
		}

		if (error > 0)
		{
			--x;
			tx += 2;
			error += (tx - diameter);
		}
	}
}


void EasySDL2::_drawCircleFilled(SDL_Renderer* renderer, int centreX, int centreY, int radius)
{
	int offsetx, offsety, d;
	int status;

	offsetx = 0;
	offsety = radius;
	d = radius - 1;
	status = 0;

	while (offsety >= offsetx) {

		SDL_RenderDrawLine(renderer, centreX - offsety, centreY + offsetx, centreX + offsety, centreY + offsetx);
		SDL_RenderDrawLine(renderer, centreX - offsetx, centreY + offsety, centreX + offsetx, centreY + offsety);
		SDL_RenderDrawLine(renderer, centreX - offsetx, centreY - offsety, centreX + offsetx, centreY - offsety);
		SDL_RenderDrawLine(renderer, centreX - offsety, centreY - offsetx, centreX + offsety, centreY - offsetx);


		if (d >= 2 * offsetx) {
			d -= 2 * offsetx + 1;
			offsetx += 1;
		}
		else if (d < 2 * (radius - offsety)) {
			d += 2 * offsety - 1;
			offsety -= 1;
		}
		else {
			d += 2 * (offsety - offsetx - 1);
			offsety -= 1;
			offsetx += 1;
		}
	}
}





#endif // !EASYSDL2_H_