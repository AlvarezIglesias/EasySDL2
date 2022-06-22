#pragma once
#ifndef EASYSDL2_H_
#define EASYSDL2_H_

#include "SDL.h"
#include "SDL_image.h"
#include <iostream>
#include <unordered_map>
#include <memory>

struct Color
{
	int r, g, b, a;
};

struct Point
{
	int x, y;
};

struct _Texture
{
	SDL_Texture* texture;

	_Texture(SDL_Texture* t) : texture(t) {}
	~_Texture() {
		SDL_DestroyTexture(texture);
	}

};

class Texture
{
	std::shared_ptr<_Texture> sharedTexture;
	Texture(SDL_Texture* otherT, bool otherFlipX, bool otherFlipY){
		sharedTexture = std::make_shared<_Texture>(otherT);
		flipX = otherFlipX;
		flipY = otherFlipY;
	}

	SDL_Texture* getSharedTexture() { return sharedTexture.get()->texture; }

public:	
	Texture(const Texture& otherT) {
		sharedTexture = otherT.sharedTexture;
		flipX = otherT.flipX;
		flipY = otherT.flipY;
	}
	~Texture() {}
	Texture operator=(const Texture& other) { return Texture(other); }
	
	bool flipX;
	bool flipY;
	
	friend class EasySDL2;
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
	//int _aalineRGBA(int x1, int y1, int x2, int y2, Color color, int draw_endpoint);
	//void pixelRGBAWeight(SDL_Renderer* renderer, Sint16 x, Sint16 y, Color color, int weight);


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

	void drawTexture(int x, int y, int width, int height, Texture& texture);
	void drawTexture(int x, int y, int width, int height, int angle, Texture& texture);
	void drawTexture(int x, int y, int width, int height, int xPivot, int yPivot, Texture& texture);
	void drawTexture(int x, int y, int width, int height, int xPivot, int yPivot, float angle, Texture& texture);

	void drawFilledPolygon(const int* vx, const int* vy, int n, Color c);

	void drawQuadraticBezierCurve(int xs[4], int ys[4], const int width, Color color);
	void drawQuadraticBezierCurve(int xs[4], int ys[4], const int width, Color color, bool rounded);
	void drawQuadraticBezierCurve(Point coords[4], const int width, Color color);
	void drawQuadraticBezierCurve(Point coords[4], const int width, Color color, bool rounded);

	Texture loadTexture(std::string path);

	bool checkKeyDown(std::string keyName);
	bool checkKey(std::string keyName);
	bool checkKeyUp(std::string keyName);

	bool mouseButtonDown(int buttonIndex);
	bool mouseButton(int buttonIndex);
	bool mouseButtonUp(int buttonIndex);

	void waitUntil(std::string key);
	void waitUntil(int key);

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


int _polygonComparer(const void* a, const void* b)
{
	return (*(const int*)a) - (*(const int*)b);
}

//modified from SDL2_gfx https://www.ferzkopp.net/Software/SDL2_gfx/Docs/html/index.html
void EasySDL2::drawFilledPolygon(const int* vx, const int* vy, int n, Color color)
{
	int result;
	int i;
	int y, xa, xb;
	int miny, maxy;
	int x1, y1;
	int x2, y2;
	int ind1, ind2;
	int ints;
	int* gfxPrimitivesPolyInts = NULL;
	int* gfxPrimitivesPolyIntsNew = NULL;
	int gfxPrimitivesPolyAllocated = 0;

	/*
	* Vertex array NULL check
	*/
	if (vx == NULL) {
		return;
	}
	if (vy == NULL) {
		return;
	}

	/*
	* Sanity check number of edges
	*/
	if (n < 3) {
		return;
	}

	gfxPrimitivesPolyInts = (int*)malloc(sizeof(int) * n);

	/*
	* Check temp array
	*/
	if (gfxPrimitivesPolyInts == NULL) {
		return;
	}

	/*
	* Determine Y maxima
	*/
	miny = vy[0];
	maxy = vy[0];
	for (i = 1; (i < n); i++) {
		if (vy[i] < miny) {
			miny = vy[i];
		}
		else if (vy[i] > maxy) {
			maxy = vy[i];
		}
	}

	/*
	* Draw, scanning y
	*/
	int total = 0;
	result = 0;
	for (y = miny; (y <= maxy); y++) {
		ints = 0;
		for (i = 0; (i < n); i++) {
			if (!i) {
				ind1 = n - 1;
				ind2 = 0;
			}
			else {
				ind1 = i - 1;
				ind2 = i;
			}
			y1 = vy[ind1];
			y2 = vy[ind2];
			if (y1 < y2) {
				x1 = vx[ind1];
				x2 = vx[ind2];
			}
			else if (y1 > y2) {
				y2 = vy[ind1];
				y1 = vy[ind2];
				x2 = vx[ind1];
				x1 = vx[ind2];
			}
			else {
				continue;
			}
			if (((y >= y1) && (y < y2)) || ((y == maxy) && (y > y1) && (y <= y2))) {
				gfxPrimitivesPolyInts[ints++] = ((65536 * (y - y1)) / (y2 - y1)) * (x2 - x1) + (65536 * x1);
			}
		}

		qsort(gfxPrimitivesPolyInts, ints, sizeof(int), _polygonComparer);
		total += ints;

		/*
		* Set color
		*/
		
		SDL_SetRenderDrawBlendMode(renderer, (color.a == 255) ? SDL_BLENDMODE_NONE : SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

		for (i = 0; (i < ints); i += 2) {
			xa = gfxPrimitivesPolyInts[i] + 1;
			xa = (xa >> 16) + ((xa & 32768) >> 15);
			xb = gfxPrimitivesPolyInts[i + 1] - 1;
			xb = (xb >> 16) + ((xb & 32768) >> 15);
			drawLine(xa, y, xb, y, color);
		}
	}
	free(gfxPrimitivesPolyInts);
}
void EasySDL2::drawQuadraticBezierCurve(Point coords[4], const int width, Color color) {
	int xs[4] = { coords[0].x, coords[1].x , coords[2].x , coords[3].x };
	int ys[4] = { coords[0].y, coords[1].y , coords[2].y , coords[3].y };
	drawQuadraticBezierCurve(xs, ys, width, color, false);
}

void EasySDL2::drawQuadraticBezierCurve(Point coords[4], const int width, Color color, bool rounded) {
	int xs[4] = { coords[0].x, coords[1].x , coords[2].x , coords[3].x };
	int ys[4] = { coords[0].y, coords[1].y , coords[2].y , coords[3].y };
	drawQuadraticBezierCurve(xs, ys, width, color, rounded);
}
void EasySDL2::drawQuadraticBezierCurve(int xs[4], int ys[4], int width, Color color) {
	drawQuadraticBezierCurve(xs, ys, width, color, false);
}

//A lot faster than the other option, but doesnt draw the intersection. Also doesnt draw one segment at the end of the curve
void EasySDL2::drawQuadraticBezierCurve(int xs[4], int ys[4], int width, Color color, bool round)
{
	double xu = 0.0, yu = 0.0, u = 0.0;
	int i = 0;
	double step = 0.01;

	//past normals
	int* lnx1 = (int*)malloc((1 / step) * sizeof(int) * 2);
	int* lny1 = (int*)malloc((1 / step) * sizeof(int) * 2);

	for (u = 0.0; u <= 1.0; u += step)
	{
		xu = pow(1 - u, 3) * xs[0] + 3 * u * pow(1 - u, 2) * xs[1] + 3 * pow(u, 2) * (1 - u) * xs[2]
			+ pow(u, 3) * xs[3];
		yu = pow(1 - u, 3) * ys[0] + 3 * u * pow(1 - u, 2) * ys[1] + 3 * pow(u, 2) * (1 - u) * ys[2]
			+ pow(u, 3) * ys[3];

		double dx = 3 * pow(1 - u, 2) * (xs[1] - xs[0]) + 6 * (1 - u) * u * (xs[2] - xs[1]) + 3 * u * u * (xs[3] - xs[2]);
		double dy = 3 * pow(1 - u, 2) * (ys[1] - ys[0]) + 6 * (1 - u) * u * (ys[2] - ys[1]) + 3 * u * u * (ys[3] - ys[2]);

		//normalize and rotate 90 degrees
		double m = sqrt(dx * dx + dy * dy);
		double dt = dx;
		dx = -dy / m;
		dy = dt / m;

		//construct polygon
		lnx1[i] = (xu + dx * double(width));
		lnx1[(int)((1 / step) * 2 - i - 1)] = (xu - dx * double(width));
		lny1[i] = (yu + dy * double(width));
		lny1[(int)((1 / step) * 2 - i - 1)] = (yu - dy * double(width));

		i++;
	}

	drawFilledPolygon(lnx1, lny1, (1 / step) * 2, color);

	if (round) {
		drawCircleFilled(xs[0], ys[0], width - 1, color);
		drawCircleFilled(xs[3], ys[3], width - 1, color);
	}
	

	free(lnx1);
	free(lny1);
}

/*void EasySDL2::drawQuadraticBezierCurve(int xs[4], int ys[4], int width, Color color, bool round)
{
	double xu = 0.0, yu = 0.0, u = 0.0;
	int i = 0;
	double step = 0.0001;

	//last normals
	double lnx1, lnx2, lny1, lny2;

	for (u = 0.0; u <= 1.0; u += step)
	{
		xu = pow(1 - u, 3) * xs[0] + 3 * u * pow(1 - u, 2) * xs[1] + 3 * pow(u, 2) * (1 - u) * xs[2]
			+ pow(u, 3) * xs[3];
		yu = pow(1 - u, 3) * ys[0] + 3 * u * pow(1 - u, 2) * ys[1] + 3 * pow(u, 2) * (1 - u) * ys[2]
			+ pow(u, 3) * ys[3];

		double dx = 3 * pow(1 - u, 2) * (xs[1] - xs[0]) + 6 * (1 - u) * u * (xs[2] - xs[1]) + 3 * u * u * (xs[3] - xs[2]);
		double dy = 3 * pow(1 - u, 2) * (ys[1] - ys[0]) + 6 * (1 - u) * u * (ys[2] - ys[1]) + 3 * u * u * (ys[3] - ys[2]);

		//normalize and rotate 90 degrees
		double m = sqrt(dx * dx + dy * dy);
		double dt = dx;
		dx = -dy / m;
		dy = dt / m;

		double nx1 = xu + dx * double(width);
		double nx2 = xu - dx * double(width);
		double ny1 = yu + dy * double(width);
		double ny2 = yu - dy * double(width);


		if (u != 0.0) {
			int xs[] = { lnx1 , lnx2, nx2, nx1 };
			int ys[] = { lny1 , lny2, ny2, ny1 };
			drawFilledPolygon(xs, ys, 4, color);
		}

		lnx1 = nx1;
		lnx2 = nx2;
		lny1 = ny1;
		lny2 = ny2;

		//drawLine(xu, yu, xu + dx * double(width), yu + dy * double(width), { 199,35,2,255 });
		//drawLine(xu, yu, xu - dx * double(width), yu - dy * double(width), { 199,35,2,255 });

	}
	drawCircleFilled(xs[0], ys[0], width - 1, color);
	drawCircleFilled(xs[3], ys[3], width - 1, color);
}*/

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

void EasySDL2::drawTexture(int x, int y, int width, int height, Texture& texture)
{
	drawTexture(x, y, width, height, .0f, .0f, texture);
}

void EasySDL2::drawTexture(int x, int y, int width, int height, int angle, Texture& texture)
{
	drawTexture(x, y, width, height, 0, 0, angle, texture);
}

void EasySDL2::drawTexture(int x, int y, int width, int height, int xPivot, int yPivot, Texture& texture)
{
	drawTexture(x, y, width, height, xPivot, yPivot, 0, texture);
}

void EasySDL2::drawTexture(int x, int y, int width, int height, int xPivot, int yPivot, float angle, Texture& texture)
{
	/*if (!texture.texture.use_count() == 0) {
		std::cout << "The texture you just tried to draw has already been unloaded." << std::endl;
		return;
	}*/

	SDL_Rect dest = { x,y,width,height };
	SDL_Point pivot = { xPivot, yPivot };
	SDL_RendererFlip flip = static_cast<SDL_RendererFlip>((
		texture.flipX ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE) |
		(texture.flipY ? SDL_FLIP_VERTICAL : SDL_FLIP_NONE
			));

	SDL_RenderCopyEx(renderer, texture.getSharedTexture(), NULL, &dest, angle, &pivot, flip);
}

Texture EasySDL2::loadTexture(std::string path)
{
	SDL_Surface* imgSurface = IMG_Load(path.c_str());
	SDL_Surface* optimizedImgSurface = NULL;

	optimizedImgSurface = SDL_ConvertSurface(imgSurface, surface->format, 0);
	SDL_FreeSurface(imgSurface);

	if (optimizedImgSurface == NULL) std::cout << "File not found" << std::endl;

	Texture texture(SDL_CreateTextureFromSurface(renderer, optimizedImgSurface), false, false);
	SDL_FreeSurface(optimizedImgSurface);
	return texture;
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

	SDL_SetRenderDrawColor(renderer, 21, 4, 66, 255);
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

void EasySDL2::waitUntil(std::string keyName)
{
	while (!checkKeyDown(keyName)) {
		updateKeyMaps();
		detect();
		SDL_Delay(1);
	}
}

void EasySDL2::waitUntil(int mouseIndex)
{
	while (!mouseButtonDown(mouseIndex)) {
		updateKeyMaps();
		detect();
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