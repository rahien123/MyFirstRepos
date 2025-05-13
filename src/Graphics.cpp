#include <SDL.h>
#include <SDL_Image.h>
#include "Graphics.h"
#include "def.h"


Graphics::Graphics() {
	mTexture = NULL;
	for (int i = 0; i < 5; i++) {
		BgrTextures[i] = nullptr;
	}
	
}
Graphics::~Graphics() {
	free();
}

bool Graphics::loadBackgrounds() {
	bool success = true;
	std::string bgPaths[] = {
		"assets/background1.png",
		"assets/background2.png",
		"assets/background3.png",
		"assets/background4.png",
		"assets/background5.png"
	};
	for (int i = 0; i < 5; i++) {
		BgrTextures[i] = IMG_LoadTexture(renderer, bgPaths[i].c_str());

		if (BgrTextures[i] == nullptr) {
			printf("Unable to load background %s! SDL_image Error: %s\n", bgPaths[i].c_str(), IMG_GetError());
			success = false;
		}
	}
	return success;
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

void Graphics::renderBackground(int x, int y,int health) {
	int bgIndex = 0;
	if (health <= 2) {
		bgIndex = 4;
	}
	else if (health <= 5) {
		bgIndex = 3;
	}
	else if (health <= 8) {
		bgIndex = 2;
	}
	else if (health <= 11) {
		bgIndex = 1;
	}
	else {
		bgIndex = 0;
	}
	SDL_Rect renderQuad = { x, y, SCREEN_WIDTH, SCREEN_HEIGHT };
	SDL_RenderCopy(renderer, BgrTextures[bgIndex], nullptr, &renderQuad);
}

void Graphics::renderCards(int x, int y) {
	SDL_Rect dest;
	dest.x = x;
	dest.y = y;
	dest.w = 64;
	dest.h = 96;
	SDL_RenderCopy(renderer, mTexture, NULL, &dest);
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

		for (int i = 0; i < 5; i++) {
			if (BgrTextures[i] != nullptr) {
				SDL_DestroyTexture(BgrTextures[i]);
				BgrTextures[i] = nullptr;
			}
		}

}