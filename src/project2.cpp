#include <SDL.h>
#include <SDL_Image.h>
#include <stdio.h>
#include <iostream>
#include "Graphics.h"
#include "player.h"
#include "map.h"
#include "RandomObstacle.h"
using namespace std;


int main(int argc, char* args[]) {
	init();

	Graphics background;
	Player player;
	MapManager map;
	Obstacle obstacle;

	if (!gTileTexture.loadTexture("assets/tileset2.png")) {
		logErrorAndExit("load tile texture sprite failed", IMG_GetError());
	}
	if (!background.loadBackgrounds()) {
		logErrorAndExit("load background failed", IMG_GetError());
	}
	
	
	bool quit = false;
	map.setObstacleManager(&obstacle);
	map.initMaps();
	while (!quit) {
		while (SDL_PollEvent(&event) != 0) {
			if (event.type == SDL_QUIT) quit = true;

			player.HandleEvent(event);
			
		}
		background.prepareScene();
		background.renderBackground(0, 0,player.getPlayerHealth());

		

		player.move(*map.getCurrentMap(player.getPlayerBox().x));


		map.updateMapsIfNeeded(player.getPlayerBox().x);
		map.renderAllMaps(player.getCamera());

		obstacle.checkCollisionWith(player);
		obstacle.updateAll(player.getCamera());
		obstacle.renderAll(player.getCamera());

		player.render();

		background.presentScene();
		SDL_Delay(16);

	}

	close();
	return 0;
}


