#pragma once
#include <SDL.h>
#include <SDL_Image.h>
#include <vector>
#include "Graphics.h"
#include "def.h"
#include "map.h"

class Player {
public:
    Player();
    ~Player();

    bool loadSprites();
    void free();

    void setCollisionStatus(Map& tileMap);

    void setCamera();
    SDL_Rect& getCamera() {
        return mCamera;
    }

    void HandleEvent(SDL_Event &event,Map& tilemap);
    void move(Map& tilemap);
    void render();
private:
    Graphics sprites[4];
    int currentFrame = 0;

    int PLAYER_WIDTH = 54;
    int PLAYER_HEIGHT = 98;

    int frameCount = 0;
 
    SDL_Rect mPlayerBox;
    SDL_Rect mCamera;

    int speedX = 4.0f;
    int velX = speedX;
    int velY = 0.0f;

    bool onGround = false;
    bool touchingWallLeft = false;
    bool touchingWallRight = false;
    bool touchingCeiling = false;
    bool isSliding = false;
    SDL_RendererFlip flip = SDL_FLIP_NONE;
    
   
};