#pragma once
#ifndef EASYSDL2_H_
#define EASYSDL2_H_

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"

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

struct Font
{
	Font(const Font& otherF) {
		sharedFont = otherF.sharedFont;
	}
	~Font() {}
	Font operator=(const Font& other) { return Font(other); }
private:

	struct _Font
	{
		TTF_Font* font;
		_Font(TTF_Font* s) : font(s) {}
		~_Font() {
			TTF_CloseFont(font);
		}
	};

	std::shared_ptr<_Font> sharedFont;
	Font(TTF_Font* otherF) {
		sharedFont = std::make_shared<_Font>(otherF);
	}
	TTF_Font* getSharedFont() { return sharedFont.get()->font; }

	friend class EasySDL2;
};

struct Music
{
	Music(const Music& otherM) {
		sharedMusic = otherM.sharedMusic;
	}
	~Music() {}
	Music operator=(const Music& other) { return Music(other); }
private:

	struct _Music
	{
		Mix_Music* music;
		_Music(Mix_Music* s) : music(s) {}
		~_Music() {
			Mix_FreeMusic(music);
		}
	};

	std::shared_ptr<_Music> sharedMusic;
	Music(Mix_Music* otherM) {
		sharedMusic = std::make_shared<_Music>(otherM);
	}
	Mix_Music* getSharedSound() { return sharedMusic.get()->music; }

	friend class EasySDL2;
};


struct Sound
{
	Sound(const Sound& otherS) {
		sharedSound = otherS.sharedSound;
	}
	~Sound() {}
	Sound operator=(const Sound& other) { return Sound(other); }
private:

	struct _Sound
	{
		Mix_Chunk* sound;
		_Sound(Mix_Chunk* s) : sound(s) {}
		~_Sound() {
			Mix_FreeChunk(sound);
		}
	};

	std::shared_ptr<_Sound> sharedSound;
	Sound(Mix_Chunk* otherS) {
		sharedSound = std::make_shared<_Sound>(otherS);
	}
	Mix_Chunk* getSharedSound() { return sharedSound.get()->sound; }

	friend class EasySDL2;
};

struct Texture
{
	bool flipX;
	bool flipY;

	Texture(const Texture& otherT) {
		sharedTexture = otherT.sharedTexture;
		flipX = otherT.flipX;
		flipY = otherT.flipY;
	}
	~Texture() {}
	Texture operator=(const Texture& other) { return Texture(other); }

private:

	struct _Texture
	{
		SDL_Texture* texture;
		_Texture(SDL_Texture* t) : texture(t) {}
		~_Texture() {
			SDL_DestroyTexture(texture);
		}
	};

	std::shared_ptr<_Texture> sharedTexture;
	Texture(SDL_Texture* otherT, bool otherFlipX, bool otherFlipY) {
		sharedTexture = std::make_shared<_Texture>(otherT);
		flipX = otherFlipX;
		flipY = otherFlipY;
	}

	SDL_Texture* getSharedTexture() { return sharedTexture.get()->texture; }

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
		//SDL_FreeSurface(surface); // "Do not free this surface" https://wiki.libsdl.org/SDL3/SDL_GetWindowSurface
		Mix_Quit();
		IMG_Quit();
		TTF_Quit();
		SDL_Quit();
	};

	void drawLine(int x1, int y1, int x2, int y2);
	void drawLine(int x1, int y1, int x2, int y2, Color color);

	void drawRect(int x, int y, int width, int height, Color color);
	void drawRect(int x, int y, int width, int height, int xPivot, int yPivot, Color color);

	void drawRectFilled(int x, int y, int width, int height, Color color);
	void drawRectFilled(int x, int y, int width, int height, int xPivot, int yPivot, Color color);

	void drawCircle(int x, int y, int radius, Color color);
	void drawCircleFilled(int x, int y, int radius, Color color);

	void drawTexture(Texture& texture, int x, int y, int width, int height);
	void drawTexture(Texture& texture, int x, int y, int width, int height, int angle);
	void drawTexture(Texture& texture, int x, int y, int width, int height, int xPivot, int yPivot);
	void drawTexture(Texture& texture, int x, int y, int width, int height, int xPivot, int yPivot, float angle);

	Texture loadTexture(std::string path);

	Sound loadSound(std::string path);
	void playSound(Sound& sound);
	void setSoundVolume(Sound& sound, int volume);
	int getSoundVolume(Sound& sound);

	Music loadMusic(std::string path);
	void playMusic(Music& music);
	void playMusic(Music& music, int startingPoint);
	void playMusicFadeIn(Music& music, int fadeDuration);
	void playMusicFadeIn(Music& music, int fadeDuration, int startingPoint);
	void stopMusic();
	void stopMusicFadeOut(int fadeDuration);
	void setMusicVolume(int volume);
	int getMusicVolume();

	Font loadFont(std::string path, int size);
	void drawText(Font& font, std::string text, int x, int y);
	void drawText(Font& font, std::string text, int x, int y, int angle);
	void drawText(Font& font, std::string text, int x, int y, int xPivot, int yPivot, float angle);

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
#endif