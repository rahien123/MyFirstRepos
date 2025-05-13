#include <SDL.h>
#include <SDL_Image.h>
#include "map.h"
#include "RandomObstacle.h"
#include "player.h"

Graphics gTileTexture;
SDL_Rect gTileClips[TOTAL_TILE_SPRITES];

void setTileClips() {
	gTileClips[TILE_POLE].x = 0;
	gTileClips[TILE_POLE].y = 0;
	gTileClips[TILE_POLE].w = TILE_WIDTH;
	gTileClips[TILE_POLE].h = TILE_HEIGHT;

	gTileClips[TILE_PAVEMENT].x = 64;
	gTileClips[TILE_PAVEMENT].y = 0;
	gTileClips[TILE_PAVEMENT].w = TILE_WIDTH;
	gTileClips[TILE_PAVEMENT].h = TILE_HEIGHT;

	gTileClips[TILE_BUILDING].x = 0;
	gTileClips[TILE_BUILDING].y = 64;
	gTileClips[TILE_BUILDING].w = TILE_WIDTH;
	gTileClips[TILE_BUILDING].h = TILE_HEIGHT;

	gTileClips[TILE_ROAD].x = 64;
	gTileClips[TILE_ROAD].y = 64;
	gTileClips[TILE_ROAD].w = TILE_WIDTH;
	gTileClips[TILE_ROAD].h = TILE_HEIGHT;
}

Tile::~Tile() {};
Tile::Tile(int x, int y, int tileType)
{
    mBox.x = x;
    mBox.y = y;

    
    mBox.w = TILE_WIDTH;
    mBox.h = TILE_HEIGHT;

   
    mType = tileType;
}

void Tile::render(SDL_Rect& camera)
{
	if (checkCollision(mBox, camera)) {
		if (mType != 0) {
			gTileTexture.renderTexture(mBox.x - camera.x, mBox.y - camera.y, &gTileClips[mType]);
		}
	}
	
    
}

int Tile::getType()
{
    return mType;
}

SDL_Rect Tile::getBox()
{
    return mBox;
}

Map::Map() {
	START_MAP_X = 0;
	START_MAP_Y = 0;
	for (int i = 0; i < TOTAL_TILES; i++) {
		tileSet[i] = nullptr;
	}

}
Map::Map(int x, int y) {
	START_MAP_X = x;
	START_MAP_Y = y;
}
Map::~Map() {
	freeTile();
}

void Map::freeTile() {
	for (int i = 0; i < TOTAL_TILES; i++) {
		if (tileSet[i] != NULL) {
			delete tileSet[i];
			tileSet[i] = NULL;
		}
	}

}

bool Map::setTiles(const char* mappath,Obstacle& obstacleSystem) {

	bool tilesLoaded = true;

	int x = START_MAP_X, y = START_MAP_Y;

	std::ifstream map(mappath);

	if (map.fail())
	{
		printf("Unable to load map file!\n");
		tilesLoaded = false;
	}
	else
	{

		for (int i = 0; i < TOTAL_TILES; ++i)
		{

			int tileType = -1;


			map >> tileType;


			if (map.fail())
			{
				
				printf("Error loading map: Unexpected end of file!\n");
				tilesLoaded = false;
				break;
			}

			if ((tileType >= 0) && (tileType < TOTAL_TILE_SPRITES))
			{
				tileSet[i] = new Tile(x, y, tileType);
			}

			else
			{

				printf("Error loading map: Invalid tile type at %d!\n", i);
				tilesLoaded = false;
				break;
			}

			x += TILE_WIDTH;

			if (x >= LEVEL_WIDTH + START_MAP_X)
			{
				//Move back
				x = START_MAP_X;

				y += TILE_HEIGHT;
			}
		}

		std::string typeStr;
		int sx, sy, ex, ey;
		// Read obstacle lines
		while (map >> typeStr >> sx >> sy >> ex >> ey) {
			ObstacleType type;

			// Adjust positions by map offset
			sx += START_MAP_X;
			sy += START_MAP_Y;
			ex += START_MAP_X;
			ey += START_MAP_Y;
			if (typeStr == "MOVING") {
				type = ObstacleType::MOVING;

				if (obstacleSystem.movingTexture) {
					obstacleSystem.obstacles.emplace_back(obstacleSystem.movingTexture,sx, sy, ex, ey,OBSTACLE_SPEED,type);
				}
			}
			else if (typeStr == "BOOMERANG") {
				
				type = ObstacleType::BOOMERANG;

				if (obstacleSystem.boomerangTexture && obstacleSystem.throwerTexture) {
					obstacleSystem.obstacles.emplace_back(obstacleSystem.boomerangTexture,sx, sy, ex, ey,OBSTACLE_SPEED,type,obstacleSystem.throwerTexture);
				}
			}
		}

		map.close();

		return tilesLoaded;

	}
}

void Map::drawMap(SDL_Rect& camera) {
	
		for (int i = 0; i < TOTAL_TILES; i++) {
			if (tileSet[i] != NULL) 
			tileSet[i]->render(camera);
			else {
				printf("tileSet[%d] is nullptr\n", i);
			}
		}
	

}

MapManager::MapManager() {
	mapFiles = { "map1.txt", "map2.txt", "map3.txt", "map4.txt", "map5.txt", "map6.txt" };
	setTileClips();
}

MapManager::~MapManager() {
	for (Map* map : loadedMaps) {
		delete map;
	}
	loadedMaps.clear();
}

void MapManager::initMaps() {
	if (!obstacleManager) {
		printf("Warning: Obstacle manager not set in MapManager!\n");
		return;
	}

	std::shuffle(mapFiles.begin(), mapFiles.end(), std::default_random_engine(std::time(0)));

	for (int i = 0; i < 3; ++i) {
		Map* map = new Map(i * LEVEL_WIDTH, 0);
		cout << mapFiles[i].c_str() << endl;
		map->setTiles(("assets/"+ mapFiles[i]).c_str(),*obstacleManager);
		loadedMaps.push_back(map);
	}
}


void MapManager::updateMapsIfNeeded(int playerX) {
	if (!obstacleManager) return;

	int currentMapIndex = (playerX / LEVEL_WIDTH);

	
	if (currentMapIndex == loadedMaps.back()->getStartX() / LEVEL_WIDTH) {
		
		int mapToRemoveStartX = loadedMaps.front()->getStartX();

		delete loadedMaps.front();
		loadedMaps.erase(loadedMaps.begin());

		obstacleManager->removeObstaclesForMap(mapToRemoveStartX);
		
		std::string newMap = mapFiles[rand() % mapFiles.size()];
		int newX = loadedMaps.back()->getStartX() + LEVEL_WIDTH;
		cout << newMap << endl;
		Map* newMapPtr = new Map(newX, 0);
		if (newMapPtr->setTiles(("assets/" + newMap).c_str(),*obstacleManager)) {
			loadedMaps.push_back(newMapPtr);
		}
		else {
			delete newMapPtr;
		}
	}
}


void MapManager::renderAllMaps(SDL_Rect& camera) {
	for (Map* map : loadedMaps) {
		map->drawMap(camera);

	}
}

Map* MapManager::getCurrentMap(int playerX) {
	if (playerX <= 0) return loadedMaps[0];
	for (Map* map : loadedMaps) {
		int startX = map->getStartX();
		int endX = startX + LEVEL_WIDTH;
		if (playerX >= startX && playerX < endX) {
			return map;
		}
	}
	return nullptr;
}

