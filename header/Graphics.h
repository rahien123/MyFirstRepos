#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_Image.h>
using namespace std;

class Graphics {
public:
	Graphics();
	~Graphics();

	
	void free();
	
	void renderText(const std::string& text, int x, int y, SDL_Color color);

	bool loadTexture(const char* filepath);
	bool loadBackgrounds();

	void prepareScene();

	void renderTexture(int x,int y,SDL_Rect* clip= NULL,SDL_RendererFlip flip=SDL_FLIP_NONE);
	void renderPlayer(int x, int y, SDL_Rect* clip = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
	void renderBackground(int x, int y,int health);
	void renderCards(int x, int y);
	void presentScene();

	
private:
	SDL_Texture* mTexture;
	SDL_Texture* BgrTextures[5];
};

