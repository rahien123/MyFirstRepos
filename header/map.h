#pragma once
#include <SDL.h>
#include <SDL_Image.h>
#include <fstream>
#include "Graphics.h"
#include "def.h"

extern Graphics gTileTexture;
extern SDL_Rect gTileClips[TOTAL_TILE_SPRITES];

void setTileClips();
class Tile
{
public:
	
	Tile(int x, int y, int tileType);
	~Tile();
	void render(SDL_Rect& camera);

	int getType();

	
	SDL_Rect getBox();

private:
	SDL_Rect mBox;
	int mType;
};

class Map {
public:
	Map();
	~Map();
	void freeTile();
	bool setTiles(const char* mappath);
	void drawMap(SDL_Rect& camera);
	Tile* tileSet[TOTAL_TILES] ;
private:
	int START_MAP_X;
	int START_MAP_Y;
	
};


