#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <algorithm>

#include "def.h"
#include <vector>
#include <string>


enum class ObstacleType { FALLING, FLYING ,MOVING,BOOMERANG};

class Player;
class Map;

class SingleObstacle {
public:
    // Constructor normal obstacles (falling, flying)
    SingleObstacle(SDL_Texture* _texture, int x, int y, float vx, float vy, ObstacleType _type);

    // constructor moving and boomerang 
    SingleObstacle(SDL_Texture* _texture, int _startX, int _startY, int _endX, int _endY,
        float speed, ObstacleType _type, SDL_Texture* _throwerTexture = nullptr);
    int getMapStartX() const {
        return mapStartX;
    }

    void update();
    void render(const SDL_Rect& camera);
    bool isOffScreen(const SDL_Rect& camera) const;
    bool checkCollision(Player& player);
    ObstacleType getType() const { return type; }

private:
    SDL_Texture* texture;
    SDL_Rect rect;//postition
    float velocityX;
    float velocityY;
    ObstacleType type;

    //MOVING OBSTACLE
    int startX, startY;
    int endX, endY;
    bool movingToEnd;
    
    //BOOMERANG
    SDL_Texture* throwerTexture;
    SDL_Rect throwerRect;
    float rotationAngle;
    SDL_RendererFlip flip;
    bool visible = true;

    int mapStartX;

    bool hasCollided;
    Uint32 lastCollisionTime;
};

class Obstacle {
public:
    friend class Map;

    Obstacle();
    ~Obstacle();

    void loadTextures();
    void updateAll(const SDL_Rect& camera);
    void renderAll(const SDL_Rect& camera);
    void checkCollisionWith(Player& player);

    void spawnRandomObstacle(const SDL_Rect& camera);

    void removeObstaclesForMap(int mapStartX);
    void clearAll() {
        obstacles.clear();
    }

    SDL_Texture* loadTexture(const std::string& path);
private:
    std::vector<SingleObstacle> obstacles;
    std::vector<SDL_Texture*> fallingTextures;
    std::vector<SDL_Texture*> flyingTextures;

    SDL_Texture* movingTexture;
    SDL_Texture* boomerangTexture;
    SDL_Texture* throwerTexture;
    Uint32 lastSpawnTime;
 
};
