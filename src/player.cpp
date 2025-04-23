#include "player.h"
Player::Player() {
    // Gọi loadSprites nếu cần
    loadSprites();
    mPlayerBox.x = 128;
    mPlayerBox.y = TILE_HEIGHT * 15 - PLAYER_HEIGHT;
    mPlayerBox.w = PLAYER_WIDTH;
    mPlayerBox.h = PLAYER_HEIGHT;

    setCamera();

}
Player::~Player() {
    free();
}

bool Player::loadSprites() {

    if (!sprites[0].loadTexture("assets/run1.png")) {
        printf("Unable to load run1.png\n");
        return false;
    }
    if (!sprites[1].loadTexture("assets/run2.png")) {
        printf("Unable to load run1.png\n");
        return false;
    }
    if (!sprites[2].loadTexture("assets/jump.png")) {
        printf("Unable to load run1.png\n");
        return false;
    }
    if (!sprites[3].loadTexture("assets/slide.png")) {
        printf("Unable to load run1.png\n");
        return false;
    }
    return true;
}

void Player::free() {
    for (int i = 0; i < TOTAL_TILE_SPRITES; i++) {
        sprites[i].free();
    }
}

void Player::setCamera() {
    mCamera.x = (mPlayerBox.x + PLAYER_WIDTH / 2) - SCREEN_WIDTH / 2;
    mCamera.y = (mPlayerBox.y + PLAYER_HEIGHT / 2) - SCREEN_HEIGHT / 2;

    //Keep the camera in bounds
    if (mCamera.x < 0)
    {
        mCamera.x = 0;
    }
    if (mCamera.y < 0)
    {
        mCamera.y = 0;
    }
    if (mCamera.x > LEVEL_WIDTH - SCREEN_WIDTH)
    {
        mCamera.x = LEVEL_WIDTH - SCREEN_WIDTH;
    }
    if (mCamera.y > LEVEL_HEIGHT - SCREEN_HEIGHT)
    {
        mCamera.y = LEVEL_HEIGHT - SCREEN_HEIGHT;
    }

    mCamera.w = SCREEN_WIDTH;
    mCamera.h = SCREEN_HEIGHT;
}

void Player::HandleEvent(SDL_Event& event) {
    
    if (event.type == SDL_MOUSEBUTTONDOWN || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE)) {
        //hiệu ứng nhảy
        currentFrame = 2;
        if (onGround || isSliding) {
            velY = JUMP_FORCE;

            if (isSliding) {
                if (touchingWallLeft) {
                    velX = speedX;
                    flip = SDL_FLIP_NONE;
                }
                else if (touchingWallRight) {
                    velX = -speedX;
                    flip = SDL_FLIP_HORIZONTAL;
                }
            }
        }
    }
}


bool Player::checkCollisionWithLeftWall(Map& tileMap) {
 
    for (int i = 0; i < TOTAL_TILES; ++i) {
        Tile* tile = tileMap.tileSet[i];
        SDL_Rect box = tile->getBox();
        if (!checkCollision(box, getCamera())) continue;
        if (tile->getType() == TILE_BUILDING || tile->getType() == TILE_ROAD) {
            if (
                mPlayerBox.x + velX < box.x + box.w &&
                mPlayerBox.x + velX > box.x + box.w - 10 &&
                mPlayerBox.y + mPlayerBox.h > box.y &&
                mPlayerBox.y < box.y + box.h
                ) {
                // Snap player ra bên phải tile
                mPlayerBox.x = box.x + box.w;
                
                return true;
            }
        }
    }
    return false;
    }

bool Player::checkCollisionWithGround(Map& tileMap) {

    for (int i = 0; i < TOTAL_TILES; ++i) {
        Tile* tile = tileMap.tileSet[i];
        SDL_Rect box = tile->getBox();
        if (!checkCollision(box, getCamera())) continue;
        if (tile->getType() == TILE_BUILDING || tile->getType() == TILE_ROAD) {
            // Kiểm tra nếu đáy player gần mặt của tile (với một độ sai số nhỏ)
            if (
                mPlayerBox.y + mPlayerBox.h + velY >= box.y-15 &&  // Đáy player chạm vào mặt trên của tile
                mPlayerBox.y + mPlayerBox.h + velY <= box.y + 5 && // Không cách quá xa
                mPlayerBox.x + mPlayerBox.w > box.x &&  // Kiểm tra va chạm theo chiều ngang
                mPlayerBox.x < box.x + box.w
                ) {
                // Snap player lên trên tile (đặt lại vị trí y của player)
                mPlayerBox.y = box.y - mPlayerBox.h;  // Đặt y đúng vào mặt đất
                return true;
            }
        }
    }
    return false;
}



bool Player::checkCollisionWithRightWall(Map& tileMap) {
 

    for (int i = 0; i < TOTAL_TILES; ++i) {
        Tile* tile = tileMap.tileSet[i];
        SDL_Rect box = tile->getBox();
        if (!checkCollision(box, getCamera())) continue;
        if (tile->getType() == TILE_BUILDING || tile->getType() == TILE_ROAD ) {
            if (
                mPlayerBox.x + mPlayerBox.w + velX > box.x &&
                mPlayerBox.x + mPlayerBox.w + velX< box.x + 10 && // sai số nhỏ
                mPlayerBox.y + mPlayerBox.h > box.y &&
                mPlayerBox.y < box.y + box.h
                ) {
                // Snap player ra bên trái tile
                mPlayerBox.x = box.x - mPlayerBox.w;
                
                return true;
            }
        }
    }
    return false;
}

bool Player::checkCollisionWithCeiling(Map& tileMap) {
    //tránh vượt ngoài map
    if (mPlayerBox.y+velY < 0) {
        mPlayerBox.y = 0;
        velY = 0;
        return true;
    }

    for (int i = 0; i < TOTAL_TILES; ++i) {
        Tile* tile = tileMap.tileSet[i];
        SDL_Rect box = tile->getBox();
        if (!checkCollision(box, getCamera())) continue;
        if (tile->getType() == TILE_BUILDING) {
            if( mPlayerBox.x + mPlayerBox.w > box.x && 
                mPlayerBox.x < box.x + box.w &&
                mPlayerBox.y + velY < box.y + box.h &&
                mPlayerBox.y >= box.y + box.h )

            {
                mPlayerBox.y = box.y + box.h;
                return true;
            }
        }
    }

    return false;
}

void Player::setCollisionStatus(Map& tilemap) {

}



void Player::move(Map& tileMap) {

    // Tính toán vật lý 
    mPlayerBox.x += velX;
    velY += GRAVITY;
    //phải ở trên xử lí va chạm tường nếu không lúc trượt trên tường,bấm nhảy x luôn không đổi vì x luôn bị -velX rồi lại +velX
    // 
    //  Va chạm tường 
    touchingWallLeft = checkCollisionWithLeftWall(tileMap);
    touchingWallRight = checkCollisionWithRightWall(tileMap);

    if (touchingWallLeft)
        mPlayerBox.x -= velX;
    if (touchingWallRight)
        mPlayerBox.x -= velX;

    //  Wall slide 
    isSliding = false;
    if (!onGround && (touchingWallLeft ||touchingWallRight)) {
        if (velY > WALL_SLIDE_SPEED)
            velY = WALL_SLIDE_SPEED;
        isSliding = true;
        currentFrame = 3;
    }

    //  kiểm tra chạm đầu->Cập nhật y ->kiểm tra chạm đất
    if (checkCollisionWithCeiling(tileMap)) {
        mPlayerBox.y -= velY; // Hoàn lại vì đã snap bên trong hàm
        velY = 0;
    }

    mPlayerBox.y += velY;
        //phải ở trên check chạm đất vì khi đang chạm đất 

 
    if (checkCollisionWithGround(tileMap)) {
        mPlayerBox.y -= velY;
        velY = 0;
        onGround = true;
        frameCount++;
        if (currentFrame > 1) currentFrame = 0;
        if (frameCount % 5 == 0) {
            currentFrame = (currentFrame + 1) % 2;
        }

    }
    else {
        onGround = false;

    }
  
    //hiệu ứng rơi=hiệu ứng nhảy
    if (!onGround && !isSliding) {
        currentFrame = 2;
    }
    
    setCamera();
}

void Player::render() {
    sprites[currentFrame].renderPlayer(mPlayerBox.x - mCamera.x, mPlayerBox.y - mCamera.y, NULL, flip);
}