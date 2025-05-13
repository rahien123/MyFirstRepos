#include "RandomObstacle.h"
#include "player.h"
#include "Map.h" 

SingleObstacle::SingleObstacle(SDL_Texture* _texture, int x, int y, float vx, float vy, ObstacleType _type)
    : texture(_texture), velocityX(vx), velocityY(vy), type(_type),
    startX(0), startY(0), endX(0), endY(0),
    movingToEnd(true), rotationAngle(0.0f),
    throwerTexture(nullptr),
    flip(SDL_FLIP_NONE) {
    rect = { x, y, 64, 64 };
    throwerRect = { 0, 0, 0, 0 };
}

SingleObstacle::SingleObstacle(SDL_Texture* _texture, int _startX, int _startY, int _endX, int _endY,
    float speed, ObstacleType _type, SDL_Texture* _throwerTexture)
    : texture(_texture), type(_type),
    startX(_startX), startY(_startY), endX(_endX), endY(_endY),
    movingToEnd(true), rotationAngle(0.0f),
    throwerTexture(_throwerTexture),
    flip(SDL_FLIP_NONE) {

    //obstacle dang o map nao
    mapStartX = (startX / LEVEL_WIDTH) * LEVEL_WIDTH;

    rect = { startX, startY, 64, 64 };

    if (throwerTexture != nullptr) {
        throwerRect = { startX, startY, 64, 64 };
    }

    if (startY == endY) {
        // Horizontal movement (startX < endX)
        velocityX = speed;
        velocityY = 0;
    }
    else if (startX == endX) {
        // Vertical movement (startY > endY)
        velocityX = 0;
        velocityY = -speed; 
    }
}

void SingleObstacle::update() {
    if (type == ObstacleType::FALLING || type == ObstacleType::FLYING) {
        rect.x += static_cast<int>(velocityX);
        rect.y += static_cast<int>(velocityY);
        return;
    }

    if (type == ObstacleType::MOVING|| type == ObstacleType::BOOMERANG) {

        rect.x += static_cast<int>(velocityX);
        rect.y += static_cast<int>(velocityY);

        // Check horizontal movement (startX < endX)
        if (velocityX != 0) {
            if (movingToEnd) {
                // Moving from start to end (right)
                if (rect.x >= endX) {
                    rect.x = endX; 
                    velocityX = -velocityX;
                    movingToEnd = false;

                   
                    if (type == ObstacleType::MOVING) {
                        flip = (flip == SDL_FLIP_NONE) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
                    }
                }
            }
            else {
                // Moving from end to start (left)
                if (rect.x <= startX) {
                    rect.x = startX; 
                    velocityX = -velocityX;
                    movingToEnd = true;

             
                    if (type == ObstacleType::MOVING) {
                        flip = (flip == SDL_FLIP_NONE) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
                    }
                }
            }
        }
        // Check vertical movement (startY > endY)
        else if (velocityY != 0) {
            if (movingToEnd) {
                // Moving from start to end (up)
                if (rect.y <= endY) {
                    rect.y = endY;
                    velocityY = -velocityY;
                    movingToEnd = false;

                    if (type == ObstacleType::MOVING) {
                        flip = (flip == SDL_FLIP_NONE) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
                    }
                }
            }
            else {
                // Moving from end to start (down)
                if (rect.y >= startY) {
  
                    rect.y = startY;  
                    velocityY = -velocityY;
                    movingToEnd = true;

   
                    if (type == ObstacleType::MOVING) {
                        flip = (flip == SDL_FLIP_NONE) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
                    }
                }
                
            }
        }

        if (type == ObstacleType::BOOMERANG) {
            rotationAngle += 20.0f;
            if (rotationAngle > 360.0f) {
                rotationAngle -= 360.0f;
            }
        }
    }
}


void SingleObstacle::render(const SDL_Rect& camera) {
    // Adjust position relative to camera
    SDL_Rect renderRect = rect;
    renderRect.x -= camera.x;
    renderRect.y -= camera.y;
    switch (type) {
    case ObstacleType::BOOMERANG:
        // Render the thrower first if we have one
        if (throwerTexture) {
            SDL_Rect throwerRenderRect = throwerRect;
            throwerRenderRect.x -= camera.x;
            throwerRenderRect.y -= camera.y;

            // Render thrower with appropriate flip
            SDL_RenderCopyEx(renderer, throwerTexture, nullptr, &throwerRenderRect,
                0.0, nullptr, flip);

            int distanceX = abs(rect.x - throwerRect.x);
            int distanceY = abs(rect.y - throwerRect.y);

            // Nếu boomerang đang quay về và đủ gần thrower, không render
            if (distanceX < 64 && distanceY < 64) {
                break; 
            }
        }

        // Render boomerang with rotation
        
            SDL_RenderCopyEx(renderer, texture, nullptr, &renderRect,
                rotationAngle, nullptr, SDL_FLIP_NONE);
            break;
        

    case ObstacleType::MOVING:
        // Render with appropriate flip
        SDL_RenderCopyEx(renderer, texture, nullptr, &renderRect,
            0.0, nullptr, flip);
        break;

    default:
        // Render normal obstacles
        SDL_RenderCopy(renderer, texture, nullptr, &renderRect);
        break;
    }
}

bool SingleObstacle::isOffScreen(const SDL_Rect& camera) const {
    if (type == ObstacleType::BOOMERANG || type == ObstacleType::MOVING) {
        return false;
    }
    bool offScreen = (
        rect.x + rect.w < camera.x ||      // hoàn toàn bên trái camera
        rect.x > camera.x + camera.w ||    // hoàn toàn bên phải camera
        rect.y > camera.y + camera.h       // hoàn toàn bên dưới camera
        );
    return offScreen;
}



bool SingleObstacle::checkCollision(Player& player) {
    // Có thời gian cooldown là 1 giây
    const Uint32 COLLISION_COOLDOWN = 1000;
    Uint32 currentTime = SDL_GetTicks();

    // Nếu đã va chạm và vẫn trong thời gian cooldown thì không phát hiện va chạm nữa
    if (hasCollided && (currentTime - lastCollisionTime < COLLISION_COOLDOWN)) {
        return false;
    }

    // Kiểm tra va chạm
    SDL_Rect playerRect = player.getPlayerBox();
    bool collision = SDL_HasIntersection(&rect, &playerRect);

    // Nếu có va chạm thì cập nhật trạng thái
    if (collision) {
        hasCollided = true;
        lastCollisionTime = currentTime;
    }

    return collision;
}


Obstacle::Obstacle() : lastSpawnTime(0) {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    loadTextures();
}

Obstacle::~Obstacle() {
    for (auto texture : fallingTextures) SDL_DestroyTexture(texture);
    for (auto texture : flyingTextures) SDL_DestroyTexture(texture);
    if (movingTexture) SDL_DestroyTexture(movingTexture);
    if (boomerangTexture) SDL_DestroyTexture(boomerangTexture);
    if (throwerTexture) SDL_DestroyTexture(throwerTexture);
}

void Obstacle::updateAll(const SDL_Rect& camera) {
    Uint32 now = SDL_GetTicks();
    if (now - lastSpawnTime > 2000) {

        spawnRandomObstacle(camera);
        lastSpawnTime = now;
    }

    for (auto& o : obstacles) o.update();

    obstacles.erase(
        std::remove_if(obstacles.begin(), obstacles.end(),
            [&](const SingleObstacle& o) {
                return o.isOffScreen(camera);
            }),
        obstacles.end()
    );
}

void Obstacle::renderAll(const SDL_Rect& camera) {
    for (auto& o : obstacles)
        o.render(camera);
}

void Obstacle::checkCollisionWith(Player& player) {
    for (auto& o : obstacles) {
        if (o.checkCollision(player)) {
            player.takeDamage();
        }
    }
}


SDL_Texture* Obstacle::loadTexture(const std::string& path) {
    SDL_Texture* tex = IMG_LoadTexture(renderer, path.c_str());
    if (!tex) logErrorAndExit("failed to load image", IMG_GetError());
    return tex;
}

void Obstacle::loadTextures() {
    fallingTextures.push_back(loadTexture("assets/bomb.png"));
    fallingTextures.push_back(loadTexture("assets/dora.png"));
    fallingTextures.push_back(loadTexture("assets/meteor.png"));

    flyingTextures.push_back(loadTexture("assets/goku.png"));
    flyingTextures.push_back(loadTexture("assets/hero.png"));
    flyingTextures.push_back(loadTexture("assets/hero2.png"));

    movingTexture = loadTexture("assets/cat.png");
    boomerangTexture = loadTexture("assets/scythe.png");
    throwerTexture = loadTexture("assets/reaper2.png");
}

void Obstacle::spawnRandomObstacle(const SDL_Rect& camera) {
    int type = rand() % 2;

    if (type == 0 && !fallingTextures.empty()) {
        // Spawn obstacle rơi từ trên xuống
        SDL_Texture* tex = fallingTextures[rand() % fallingTextures.size()];
        int x = camera.x + rand() % (camera.w - 64);  // Spawn ngẫu nhiên trong vùng ngang camera
        int y = camera.y - 64;                        // Ngay trên vùng nhìn thấy

        obstacles.emplace_back(tex, x, y, 0, OBSTACLE_SPEED, ObstacleType::FALLING);
    }
    else if (!flyingTextures.empty()) {
        // Chướng ngại vật bay ngang từ phải sang trái
        SDL_Texture* tex = flyingTextures[rand() % flyingTextures.size()];
        int y = camera.y + rand() % (camera.h - 64); // spawn trong vùng camera theo chiều dọc

        // Spawn sát mép phải camera (thay vì ngoài xa)
        int x = camera.x + camera.w - 1;

        obstacles.emplace_back(tex, x, y, -OBSTACLE_SPEED, 0, ObstacleType::FLYING);
    }
}

void Obstacle::removeObstaclesForMap(int mapStartX) {
    auto it = obstacles.begin();
    while (it != obstacles.end()) {
        if (it->getMapStartX() == mapStartX) {
            it = obstacles.erase(it);
        }
        else {
            ++it;
        }
    }
}




