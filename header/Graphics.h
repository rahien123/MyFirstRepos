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
	

	bool loadTexture(const char* filepath);

	void prepareScene();

	void renderTexture(int x,int y,SDL_Rect* clip= NULL,SDL_RendererFlip flip=SDL_FLIP_NONE);
	void renderBackground(int x, int y);
	void presentScene();

	
private:
	SDL_Texture* mTexture;
	
};

