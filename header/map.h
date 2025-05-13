#pragma once
#include <SDL.h>
#include <SDL_Image.h>
#include <fstream>
#include <vector>
#include <random>    
#include <algorithm>  
#include <ctime>  
#include "Graphics.h"
#include "def.h"


extern Graphics gTileTexture;
extern SDL_Rect gTileClips[TOTAL_TILE_SPRITES];

class Obstacle;
class Player;

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
	Map(int x, int y);
	~Map();
	void freeTile();
	bool setTiles(const char* mappath, Obstacle& obstacleSystem);
	void drawMap(SDL_Rect& camera);
	int getStartX() const { return START_MAP_X; }
	Tile* tileSet[TOTAL_TILES] ;

private:
	int START_MAP_X;
	int START_MAP_Y;
	
};

class MapManager {
public:
	MapManager();
	~MapManager();

	void setObstacleManager(Obstacle* manager) { obstacleManager = manager; }
	void initMaps();
	void updateMapsIfNeeded(int playerX);
	void renderAllMaps(SDL_Rect& camera);

	Map* getCurrentMap(int playerX);
private:
	vector<std::string> mapFiles;
	vector<Map*> loadedMaps;
	Obstacle* obstacleManager;
};



