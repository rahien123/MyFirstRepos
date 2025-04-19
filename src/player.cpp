
#include "player.h"
Player::Player() {
        // Gọi loadSprites nếu cần
        loadSprites();
        mPlayerBox.x = 128;
        mPlayerBox.y = TILE_HEIGHT*13-PLAYER_HEIGHT;
        mPlayerBox.w = PLAYER_WIDTH;
        mPlayerBox.h = PLAYER_HEIGHT;

        setCamera();
        
}
Player::~Player() {
    free();
}

bool Player::loadSprites() {
	
	if (!sprites[0].loadTexture("assets/run1.png")){
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
	for (int i = 0; i < 3; i++) {
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

void Player::HandleEvent(SDL_Event& event,Map &tilemap) {
    setCollisionStatus(tilemap);
    if (event.type == SDL_KEYDOWN || event.type == SDL_MOUSEBUTTONDOWN) {
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


    void Player::setCollisionStatus(Map & tileMap) {
        touchingWallLeft = false;
        touchingWallRight = false;
        onGround = false;
        touchingCeiling = false;

        for (int i = 0; i < TOTAL_TILES; ++i) {
            Tile* tile = tileMap.tileSet[i];
            SDL_Rect box = tile->getBox();
            if (tile->getType() ==TILE_BUILDING || tile->getType()==TILE_ROAD) {
               
                if (mPlayerBox.x + mPlayerBox.w > box.x &&
                    mPlayerBox.x + mPlayerBox.w < box.x + 10 && // sai số nhỏ
                    mPlayerBox.y + mPlayerBox.h > box.y &&
                    mPlayerBox.y < box.y + box.h)
                {
                    cout << "right" << " " << i << endl;
                    touchingWallRight = true;
                }

                // Chạm tường bên trái (player bên phải tile)
                if (mPlayerBox.x < box.x + box.w &&
                    mPlayerBox.x > box.x + box.w - 10 &&
                    mPlayerBox.y + mPlayerBox.h > box.y &&
                    mPlayerBox.y < box.y + box.h)
                {
                    cout << "left" << " " << i << endl;
                    touchingWallLeft = true;
                }

                // Chạm đất
                if (
                    mPlayerBox.y + mPlayerBox.h >= box.y - 1 && // đáy player gần mặt tile
                    mPlayerBox.y + mPlayerBox.h <= box.y + 5 && // không cách quá xa
                    mPlayerBox.x + mPlayerBox.w > box.x &&
                    mPlayerBox.x < box.x + box.w) {

                    // Xác định player chạm đất
                    onGround = true;

                    // Snap player xuống để đứng trên tile
                    mPlayerBox.y = box.y - mPlayerBox.h;
                    velY = 0; // Đặt lại tốc độ rơi khi chạm đất
                    cout << "ground" << " " << i << endl;
                }

                // Chạm trần
                if (mPlayerBox.y >= box.y + box.h - 10 &&
                    mPlayerBox.y <= box.y + box.h &&
                    mPlayerBox.x + mPlayerBox.w > box.x &&
                    mPlayerBox.x < box.x + box.w)
                {
                    touchingCeiling = true;
                }
            }
        }
    }



void Player::move(Map& tilemap){
    
// ==== 2. Tính toán vật lý ====
mPlayerBox.x += velX;
velY += GRAVITY;
//phải ở trên xử lí va chạm tường nếu không lúc trượt trên tường,bấm nhảy x luôn không đổi
// 
// ==== 3. Va chạm tường ====
setCollisionStatus(tilemap);

if (touchingWallLeft)
mPlayerBox.x -= velX;
if (touchingWallRight)
mPlayerBox.x -= velX;

// ==== 4. Wall slide ====
isSliding = false;
if (!onGround && (touchingWallLeft || touchingWallRight)) {
    if (velY > WALL_SLIDE_SPEED)
       velY = WALL_SLIDE_SPEED;
    isSliding = true;
    currentFrame = 3;
}

// ==== 5. Cập nhật y và kiểm tra chạm đất ====
mPlayerBox.y += velY;
setCollisionStatus(tilemap);
if (onGround) {
    //player.y = 500 - PLAYER_HEIGHT;//sửa sau
    mPlayerBox.y -= velY;
    velY = 0;
    frameCount++;
    if (currentFrame > 1) currentFrame = 0;
    if (frameCount % 5 == 0) {
        frameCount = 0;
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
    sprites[currentFrame].renderTexture(mPlayerBox.x-mCamera.x,mPlayerBox.y- mCamera.y, NULL, flip);
}