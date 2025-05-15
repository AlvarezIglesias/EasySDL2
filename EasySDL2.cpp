
#include "EasySDL2.h"

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
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
	);
	renderer = SDL_CreateRenderer(window, -1, 0);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);

	surface = SDL_GetWindowSurface(window);
	time = SDL_GetPerformanceCounter();

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
	}
	if (TTF_Init() == -1) {
		printf("TTF_Init: %s\n", TTF_GetError());
	}

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

void EasySDL2::drawRect(int x, int y, int width, int height, int xPivot, int yPivot, Color color)
{
	SDL_Rect dest = { x - xPivot, y - yPivot,width ,height };
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_RenderDrawRect(renderer, &dest);
}

void EasySDL2::drawRectFilled(int x, int y, int width, int height, Color color)
{
	drawRectFilled(x, y, width, height, .0f, .0f, color);
}

void EasySDL2::drawRectFilled(int x, int y, int width, int height, int xPivot, int yPivot, Color color)
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


void EasySDL2::drawTexture(Texture& texture, int x, int y, int width, int height)
{
	drawTexture(texture, x, y, width, height, .0f, .0f);
}

void EasySDL2::drawTexture(Texture& texture, int x, int y, int width, int height, int angle)
{
	drawTexture(texture, x, y, width, height, 0, 0, angle);
}

void EasySDL2::drawTexture(Texture& texture, int x, int y, int width, int height, int xPivot, int yPivot)
{
	drawTexture(texture, x, y, width, height, xPivot, yPivot, 0);
}

void EasySDL2::drawTexture(Texture& texture, int x, int y, int width, int height, int xPivot, int yPivot, float angle)
{
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


Font EasySDL2::loadFont(std::string path, int size)
{
	TTF_Font* loadedFont = TTF_OpenFont(path.c_str(), size);
	if (!loadedFont) {
		printf("TTF_OpenFont: %s\n", TTF_GetError());
	}
	Font font(loadedFont);
	return font;
}
void EasySDL2::drawText(Font& font, std::string text, int x, int y) {
	drawText(font, text, x, y, 0);
}
void EasySDL2::drawText(Font& font, std::string text, int x, int y, int angle) {
	drawText(font, text, x, y, 0.f, 0.5f, angle);
}

void EasySDL2::drawText(Font& font, std::string text, int x, int y, int xPivot, int yPivot, float angle) {

	SDL_Surface* text_surface;
	if (!(text_surface = TTF_RenderUTF8_Solid(font.getSharedFont(), text.c_str(), { 0,0,0,0 }))) {
		printf("TTF_RenderUTF8_Solid: %s\n", TTF_GetError());
	}
	else {
		Texture t(SDL_CreateTextureFromSurface(renderer, text_surface), false, false);

		drawTexture(t, x, y, text_surface->w, text_surface->h, xPivot, yPivot, angle);

		SDL_FreeSurface(text_surface);
	}
}

Sound EasySDL2::loadSound(std::string path)
{
	Mix_Chunk* loadedSound = Mix_LoadWAV(path.c_str());
	if (loadedSound == NULL)
	{
		printf("Failed to load sound! SDL_mixer Error: %s\n", Mix_GetError());
	}
	Sound sound(loadedSound);
	return sound;
}

void EasySDL2::playSound(Sound& sound) {
	Mix_PlayChannel(-1, sound.getSharedSound(), 0);
}


void EasySDL2::setSoundVolume(Sound& sound, int volume) {
	Mix_VolumeChunk(sound.getSharedSound(), volume);
}
int EasySDL2::getSoundVolume(Sound& sound) {
	return Mix_VolumeChunk(sound.getSharedSound(), -1);
}

Music EasySDL2::loadMusic(std::string path)
{
	Mix_Music* loadedMusic = Mix_LoadMUS(path.c_str());
	if (loadedMusic == NULL)
	{
		printf("Failed to load music! SDL_mixer Error: %s\n", Mix_GetError());
	}
	Music music(loadedMusic);
	return music;
}

void EasySDL2::playMusic(Music& music) {
	playMusicFadeIn(music, 0, 0);
}
void EasySDL2::playMusic(Music& music, int startingPoint) {
	playMusicFadeIn(music, 0, startingPoint);
}
void EasySDL2::playMusicFadeIn(Music& music, int fadeDuration) {
	playMusicFadeIn(music, fadeDuration, 0);
}
void EasySDL2::playMusicFadeIn(Music& music, int fadeDuration, int startingPoint) {
	Mix_FadeInMusicPos(music.getSharedSound(), 1, fadeDuration, startingPoint);
}
void EasySDL2::stopMusic() {
	stopMusicFadeOut(0);
}
void EasySDL2::stopMusicFadeOut(int fadeDuration) {
	Mix_FadeOutMusic(fadeDuration);
}

void EasySDL2::setMusicVolume(int volume) {
	Mix_VolumeMusic(volume);
}
int EasySDL2::getMusicVolume() {
	return Mix_VolumeMusic(-1);
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

