﻿#pragma once
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "Graphics.h"

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 640
#define WINDOW_TITLE "Du cho tan the"


//player
const int PLAYER_WIDTH = 38;
const int PLAYER_HEIGHT = 50;
const float GRAVITY = 0.5f;
const float JUMP_FORCE = -10.0f;
const float WALL_SLIDE_SPEED = 1.5f;

//map
const int TILE_WIDTH = 64;
const int TILE_HEIGHT = 64;
const int TOTAL_TILES = 336;
const int LEVEL_WIDTH = 1344;
const int LEVEL_HEIGHT = 1024;

const int TOTAL_TILE_SPRITES = 5;
const int TILE_BLANK = 0;
const int TILE_POLE = 1;
const int TILE_PAVEMENT = 2;
const int TILE_BUILDING = 3;
const int TILE_ROAD = 4;

//obstacle
const float OBSTACLE_SPEED = 5.0f;

extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern TTF_Font* gFont;
extern SDL_Event event;
  
//score
extern int highScore;    
extern int currentScore;
const int POINTS_PER_MAP = 1000;

//music
extern Mix_Music* gMusic;           
extern Mix_Chunk* gJumpSound;     
extern Mix_Chunk* gWallSlideSound;       
extern Mix_Chunk* gHurtSound;

bool checkCollision(SDL_Rect a, SDL_Rect b);

void init();
void close();
void loadMedia();
void logErrorAndExit(const char* msg, const char* error);

void renderScore(Graphics& graphics,int playerX);
void loadHighScore();
void saveHighScore();

void playMusic();
void stopMusic();
void playJumpSound();
void playLandSound();
void playHurtSound();