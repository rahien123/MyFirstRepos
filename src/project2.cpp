#include <SDL.h>
#include <SDL_Image.h>
#include <stdio.h>
#include <iostream>
#include "Graphics.h"
#include "player.h"
#include "map.h"
using namespace std;



int main(int argc, char* args[]) {
	init();
	Graphics background;
	Player player;
	if (!background.loadTexture("assets/background.png")) {
		logErrorAndExit("load background failed", SDL_GetError());
	};
	if (!player.loadSprites()) {
		logErrorAndExit("load player sprites failed", SDL_GetError());
	};
	if (!gTileTexture.loadTexture("assets/tileset2.png")) {
		logErrorAndExit("load tile texture sprite failed", SDL_GetError());
	}
	Map map1,map2,map3,map4,map5,map6;
	if (!map1.setTiles("assets/map1.txt")) {
		logErrorAndExit("load map failed", SDL_GetError());
	}
	
	if (!map2.setTiles("assets/map2.txt")) {
		logErrorAndExit("load map failed", SDL_GetError());
	}
	setTileClips();
	bool quit = false;

	while (!quit) {
		while (SDL_PollEvent(&event) != 0) {
			if (event.type == SDL_QUIT) quit = true;

			player.HandleEvent(event);
			
		}
		background.prepareScene();
		background.renderBackground(0, 0);
		
		player.move(map2);

		map2.drawMap(player.getCamera());
		player.render();
		background.presentScene();
		SDL_Delay(16);

	}

	close();
	background.free();
	player.free();
	return 0;
}


