#pragma once
#include <SDL.h>
#include <SDL_Image.h>
#include <vector>
#include "Graphics.h"
#include "def.h"


class Map;

class Player {
public:
    Player();
    ~Player();

    bool loadSprites();
    void free();


    void setCamera();
    SDL_Rect& getCamera() {
        return mCamera;
    }
    bool checkCollisionWithGround(Map& tilemap);
    bool checkCollisionWithRightWall(Map& tileMap);
    bool checkCollisionWithLeftWall(Map& tileMap);
    bool checkCollisionWithCeiling(Map& tileMap);

    SDL_Rect getPlayerBox() {
        return mPlayerBox;
    }
    void HandleEvent(SDL_Event &event);
    void move(Map& tilemap);
    void render();

    void takeDamage() {
        if(currentHealth>0)  currentHealth--;
    }
    void renderHealthCards();
    int getPlayerHealth() {
        return currentHealth;
    }
private:
    Graphics sprites[4];
    int currentFrame = 0;

    int frameCount = 0;
 
    SDL_Rect mPlayerBox;
    SDL_Rect mCamera;

    float speedX = 3.0f;
    float velX = speedX;
    float velY = 0.0f;

    bool onGround = false;
    bool touchingWallLeft = false;
    bool touchingWallRight = false;
    bool touchingCeiling = false;
    bool isSliding = false;
    SDL_RendererFlip flip = SDL_FLIP_NONE;
    
    int currentHealth = 13;
    Graphics* cardTextures[13];
};