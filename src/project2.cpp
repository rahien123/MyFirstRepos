#include <SDL.h>
#include <SDL_Image.h>
#include <stdio.h>
#include <iostream>
#include "Graphics.h"
#include "player.h"
#include "map.h"
#include "RandomObstacle.h"
#include "Menu.h"
using namespace std;

int main(int argc, char* args[]) {
    init();

    Graphics background;
    Player player;
    MapManager map;
    Obstacle obstacle;
    Menu menu;

    if (!gTileTexture.loadTexture("assets/tileset2.png")) {
        logErrorAndExit("load tile texture sprite failed", IMG_GetError());
    }
    if (!background.loadBackgrounds()) {
        logErrorAndExit("load background failed", IMG_GetError());
    }
    if (!menu.init()) {
        logErrorAndExit("Failed to initialize menu", "");
    }

    loadHighScore();
    loadMedia();
    playMusic();

    bool quit = false;
    GameState gameState = MENU;

    map.setObstacleManager(&obstacle);
    map.initMaps();

    while (!quit) {
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) quit = true;

            switch (gameState) {
            case MENU:
                gameState = menu.handleMainMenuEvents(event);
                if (gameState == PLAYING) {
                    // Reset game khi bắt đầu chơi mới từ menu
                    player.reset(); // Reset player
                    currentScore = 0;  // Reset điểm
                }
                break;

            case PLAYING:
                player.HandleEvent(event);

                if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                    gameState = MENU;
                }
                break;

            case HELP:
                if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                    gameState = MENU;
                }
                break;

            case GAME_OVER:
                gameState = menu.handleGameOverEvents(event);
                if (gameState == PLAYING) {
                    obstacle.clearAll();
                    player.reset();
                    map.initMaps();
                    currentScore = 0; 
                }
                break;

            case EXIT:
                quit = true;
                break;
            }
        }

        background.prepareScene();

        switch (gameState) {
        case MENU:
            menu.renderMainMenu(background);
            break;

        case PLAYING:
            background.renderBackground(0, 0, player.getPlayerHealth());

            player.move(*map.getCurrentMap(player.getPlayerBox().x));
            map.updateMapsIfNeeded(player.getPlayerBox().x);

            obstacle.checkCollisionWith(player);
            obstacle.updateAll(player.getCamera());

            map.renderAllMaps(player.getCamera());
            obstacle.renderAll(player.getCamera());
            player.render();

            renderScore(background, player.getPlayerBox().x);

            // Kiểm tra nếu người chơi chết
            if (player.getPlayerHealth() <= 0) {
                gameState = GAME_OVER;
            }
            break;

        case HELP:
            menu.renderHelp(background);
            break;

        case GAME_OVER:
            menu.renderGameOver(background, currentScore, highScore);
            break;
        }
        background.presentScene();
        SDL_Delay(16);
    }
    saveHighScore();
    close();
    return 0;
}