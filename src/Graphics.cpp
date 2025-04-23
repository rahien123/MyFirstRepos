#include <SDL.h>
#include <SDL_Image.h>
#include "Graphics.h"
#include "def.h"


Graphics::Graphics() {
	mTexture = NULL;
}
Graphics::~Graphics() {
	free();
}




bool Graphics::loadTexture(const char* filepath) {
	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", filepath);
	mTexture = IMG_LoadTexture(renderer, filepath);
	if (mTexture == NULL) {
		logErrorAndExit("Failed to load texture", SDL_GetError());
		return false;
	}
	return true;
}

void Graphics::prepareScene() {
	SDL_RenderClear(renderer);
}

void Graphics::renderTexture(int x,int y,SDL_Rect* clip ,SDL_RendererFlip flip) {
	SDL_Rect dest;
	dest.x = x;
	dest.y = y;
	if (clip != NULL)
	{
		dest.w = clip->w;
		dest.h = clip->h;
	}
	else {
		SDL_QueryTexture(mTexture, NULL, NULL, &dest.w, &dest.h);
	}
	if (flip == SDL_FLIP_NONE) {
		SDL_RenderCopy(renderer, mTexture, clip, &dest);
	}
	else {
		SDL_RenderCopyEx(renderer, mTexture, clip, &dest, 0.0, NULL, flip);
	}
}

void Graphics::renderPlayer(int x, int y, SDL_Rect* clip, SDL_RendererFlip flip) {
	SDL_Rect dest;
	dest.x = x;
	dest.y = y;
	if (clip != NULL)
	{
		dest.w = clip->w;
		dest.h = clip->h;
	}
	else {
		dest.w = PLAYER_WIDTH;
		dest.h = PLAYER_HEIGHT;
	}
	if (flip == SDL_FLIP_NONE) {
		SDL_RenderCopy(renderer, mTexture, clip, &dest);
	}
	else {
		SDL_RenderCopyEx(renderer, mTexture, clip, &dest, 0.0, NULL, flip);
	}
}

void Graphics::renderBackground(int x, int y) {
	SDL_Rect dest;
	dest.x = x;
	dest.y = y;
	dest.w = SCREEN_WIDTH;
	dest.h = SCREEN_HEIGHT;
	SDL_RenderCopy(renderer,mTexture, NULL, &dest);
}
void Graphics::presentScene() {
	SDL_RenderPresent(renderer);
}

void Graphics::free() {
		if (mTexture != NULL)
		{
			SDL_DestroyTexture(mTexture);
			mTexture = NULL;
		}

}