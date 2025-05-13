#include <SDL.h>
#include <SDL_image.h>
#include "def.h"

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Event event;

void logErrorAndExit(const char* msg, const char* error) {
	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "%s:%s", msg, error);
	SDL_Quit();
}

void init()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		logErrorAndExit("SDL_Init Error", SDL_GetError());
	}

	if (!IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG)) {
		logErrorAndExit("SDL_image error:", IMG_GetError());
	}

	window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

	if (window == NULL) {
		logErrorAndExit("SDL_Window error", SDL_GetError());
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (renderer == NULL) {
		logErrorAndExit("SDL_Renderer error", SDL_GetError());
	}

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void loadMedia() {

}

void close() {
	IMG_Quit();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	renderer = NULL;
	window = NULL;
	SDL_Quit();
}

bool checkCollision(SDL_Rect a, SDL_Rect b) {
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;

	
	leftA = a.x;
	rightA = a.x + a.w;
	topA = a.y;
	bottomA = a.y + a.h;


	leftB = b.x;
	rightB = b.x + b.w;
	topB = b.y;
	bottomB = b.y + b.h;

	if (bottomA <= topB)
	{
		return false;
	}

	if (topA >= bottomB)
	{
		return false;
	}

	if (rightA <= leftB)
	{
		return false;
	}

	if (leftA >= rightB)
	{
		return false;
	}


	return true;
}