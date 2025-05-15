#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <string>
#include "Graphics.h"
#include "def.h"

enum GameState {
    MENU,       
    PLAYING,    
    HELP,       
    GAME_OVER,  
    EXIT        
};

struct Button {
    SDL_Rect rect;          
    std::string text;       
    bool isHovered;         
    bool isSound;           
    bool soundOn;           
};
class Menu {
public:
    Menu();
    ~Menu();

    bool init();

    GameState handleMainMenuEvents(SDL_Event& e);

    GameState handleGameOverEvents(SDL_Event& e);

    void renderMainMenu(Graphics& graphics);

    void renderGameOver(Graphics& graphics, int currentScore, int highScore);

    void renderHelp(Graphics& graphics);

    void checkMainMenuHover(int x, int y);

    void checkGameOverHover(int x, int y);

    bool isSoundOn() const { return soundToggle.soundOn; }

    void toggleSound();
private:
    std::vector<Button> mainMenuButtons; 
    Button soundToggle;                  
    Button retryButton;                  
    Button menuButton;                   

    SDL_Texture* mainMenuBgTexture;      
    SDL_Texture* gameOverBgTexture;      
    SDL_Texture* helpBgTexture;          
    SDL_Texture* soundOnTexture;         
    SDL_Texture* soundOffTexture;        

    Button createButton(int x, int y, int width, int height, const std::string& text);
    bool loadTextures();
    void free();
};