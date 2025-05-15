#include <SDL.h>
#include <SDL_image.h>
#include "def.h"
#include <fstream>
#include <sstream>

SDL_Window* window;
SDL_Renderer* renderer;
TTF_Font* gFont = NULL;
SDL_Event event;

Mix_Music* gMusic = NULL;
Mix_Chunk* gJumpSound = NULL;
Mix_Chunk* gLandSound = NULL;
Mix_Chunk* gHurtSound = NULL;

int highScore = 0;  
int currentScore = 0;

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

	if (TTF_Init() == -1)
	{
		logErrorAndExit("TTF_Font error", TTF_GetError());
	}

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		logErrorAndExit("SDL_mixer could not initialize", Mix_GetError());
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (renderer == NULL) {
		logErrorAndExit("SDL_Renderer error", SDL_GetError());
	}

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

}


void renderScore(Graphics& graphics, int playerX) {

	int mapIndex = playerX / LEVEL_WIDTH;
	currentScore = mapIndex * POINTS_PER_MAP;

	SDL_Color textColor = {0, 0, 0, 0 }; // Màu trắng

	std::string scoreText = "Score: " + std::to_string(currentScore);
	graphics.renderText(scoreText, SCREEN_WIDTH - 200, 20, textColor);

	std::string highscoreText = "High Score: " + std::to_string(highScore);
	graphics.renderText(highscoreText, SCREEN_WIDTH - 200, 50, textColor);
}

void loadHighScore() {
	std::ifstream file("assets/highscore.txt");
	if (file.is_open()) {
		file >> highScore;
		file.close();
	}
}

void saveHighScore() {

	if (currentScore > highScore) {
		highScore = currentScore; 

		std::ofstream file("assets/highscore.txt");
		if (file.is_open()) {
			file << highScore;
			file.close();
		}
	}
}

void loadMedia() {
	gFont = TTF_OpenFont("assets/lazy.ttf", 28);
	if (gFont == NULL) {
		logErrorAndExit("Failed to load font", TTF_GetError());
	}
	
	gMusic = Mix_LoadMUS("assets/sounds/background.mp3");
	if (gMusic == NULL) {
		printf("Failed to load background music! SDL_mixer Error: %s\n", Mix_GetError());
	}
	gJumpSound = Mix_LoadWAV("assets/sounds/jump.mp3");
	if (gJumpSound == NULL) {
		printf("Failed to load jump sound! SDL_mixer Error: %s\n", Mix_GetError());
	}
	gLandSound = Mix_LoadWAV("assets/sounds/land.mp3");
	if (gLandSound == NULL) {
		printf("Failed to load land sound! SDL_mixer Error: %s\n", Mix_GetError());
	}
	gHurtSound = Mix_LoadWAV("assets/sounds/hurt.mp3");
	if (gHurtSound == NULL) {
		printf("Failed to load hurt sound! SDL_mixer Error: %s\n", Mix_GetError());
	}
	Mix_VolumeChunk(gJumpSound, 5);
	Mix_VolumeChunk(gLandSound, 32);
	Mix_VolumeChunk(gHurtSound, 10);
	Mix_VolumeMusic(10);
}

void playMusic() {
	if (gMusic != NULL) {
		if (Mix_PlayingMusic() == 0) {
			Mix_PlayMusic(gMusic, -1);
		}
	}
}
void stopMusic() {
	Mix_HaltMusic();
}
void playJumpSound() {
	if (gJumpSound != NULL) {
		Mix_PlayChannel(-1, gJumpSound, 0);
	}
}
void playLandSound() {
	if (gLandSound != NULL) {
		Mix_PlayChannel(-1, gLandSound, 0);
	}
}
void playHurtSound() {
	if (gHurtSound != NULL) {
		Mix_PlayChannel(-1, gHurtSound, 0);
	}
}

void close() {
	Mix_FreeMusic(gMusic);
	Mix_FreeChunk(gJumpSound);
	Mix_FreeChunk(gLandSound);
	Mix_FreeChunk(gHurtSound);
	gMusic = NULL;
	gJumpSound = NULL;
	gLandSound = NULL;
	gHurtSound = NULL;
	Mix_Quit();
	TTF_CloseFont(gFont);
	gFont = NULL;
	TTF_Quit();
	stopMusic();

	IMG_Quit();;
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