#include "Menu.h"

Menu::Menu() {
    mainMenuBgTexture = NULL;
    gameOverBgTexture = NULL;
    helpBgTexture = NULL;
    soundOnTexture = NULL;
    soundOffTexture = NULL;
}

Menu::~Menu() {
    free();
}

bool Menu::init() {
    if (!loadTextures()) {
        return false;
    }

    mainMenuButtons.push_back(createButton(SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 90, 200, 60, "PLAY"));
    mainMenuButtons.push_back(createButton(SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2, 200, 60, "HELP"));
    mainMenuButtons.push_back(createButton(SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 + 90, 200, 60, "QUIT"));

    soundToggle = createButton(30, 30, 40, 40, "");
    soundToggle.isSound = true;
    soundToggle.soundOn = true;

    retryButton = createButton(SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 + 50, 200, 60, "PLAY AGAIN");
    menuButton = createButton(SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 + 130, 200, 60, "MAIN MENU");

    return true;
}

bool Menu::loadTextures() {
    try {
        mainMenuBgTexture = IMG_LoadTexture(renderer, "assets/back.jpg");
        if (mainMenuBgTexture == NULL) {
            return false;
        }

        gameOverBgTexture = IMG_LoadTexture(renderer, "assets/gameover.png");
        if (gameOverBgTexture == NULL) {
            return false;
        }

        helpBgTexture = IMG_LoadTexture(renderer, "assets/help.png");
        if (helpBgTexture == NULL) {
            return false;
        }

        soundOnTexture = IMG_LoadTexture(renderer, "assets/sound_on.png");
        if (soundOnTexture == NULL) {
            return false;
        }

        soundOffTexture = IMG_LoadTexture(renderer, "assets/sound_off.png");
        if (soundOffTexture == NULL) {
            return false;
        }
    }
    catch (const std::exception& e) {
        return false;
    }
    catch (...) {
        return false;
    }

    return true;
}

Button Menu::createButton(int x, int y, int width, int height, const std::string& text) {
    Button button;
    button.rect = { x, y, width, height };
    button.text = text;
    button.isHovered = false;
    button.isSound = false;
    button.soundOn = true;
    return button;
}

void Menu::checkMainMenuHover(int x, int y) {
    for (auto& button : mainMenuButtons) {
        button.isHovered = (x >= button.rect.x && x <= button.rect.x + button.rect.w &&
            y >= button.rect.y && y <= button.rect.y + button.rect.h);
    }
    soundToggle.isHovered = (x >= soundToggle.rect.x && x <= soundToggle.rect.x + soundToggle.rect.w &&
        y >= soundToggle.rect.y && y <= soundToggle.rect.y + soundToggle.rect.h);
}

void Menu::checkGameOverHover(int x, int y) {
    retryButton.isHovered = (x >= retryButton.rect.x && x <= retryButton.rect.x + retryButton.rect.w &&
        y >= retryButton.rect.y && y <= retryButton.rect.y + retryButton.rect.h);

    menuButton.isHovered = (x >= menuButton.rect.x && x <= menuButton.rect.x + menuButton.rect.w &&
        y >= menuButton.rect.y && y <= menuButton.rect.y + menuButton.rect.h);
}

GameState Menu::handleMainMenuEvents(SDL_Event& e) {
    if (e.type == SDL_MOUSEMOTION) {
        checkMainMenuHover(e.motion.x, e.motion.y);
    }
    else if (e.type == SDL_MOUSEBUTTONDOWN) {
        if (e.button.button == SDL_BUTTON_LEFT) {
            int mouseX = e.button.x;
            int mouseY = e.button.y;

            if (mouseX >= soundToggle.rect.x && mouseX <= soundToggle.rect.x + soundToggle.rect.w &&
                mouseY >= soundToggle.rect.y && mouseY <= soundToggle.rect.y + soundToggle.rect.h) {
                toggleSound();
                return MENU;
            }

            for (int i = 0; i < mainMenuButtons.size(); ++i) {
                if (mouseX >= mainMenuButtons[i].rect.x && mouseX <= mainMenuButtons[i].rect.x + mainMenuButtons[i].rect.w &&
                    mouseY >= mainMenuButtons[i].rect.y && mouseY <= mainMenuButtons[i].rect.y + mainMenuButtons[i].rect.h) {

                    switch (i) {
                    case 0: // Play
                        return PLAYING;
                    case 1: // Help
                        return HELP;
                    case 2: // Quit
                        return EXIT;
                    }
                }
            }
        }
    }
    return MENU;
}

GameState Menu::handleGameOverEvents(SDL_Event& e) {
    if (e.type == SDL_MOUSEMOTION) {
        checkGameOverHover(e.motion.x, e.motion.y);
    }
    else if (e.type == SDL_MOUSEBUTTONDOWN) {
        if (e.button.button == SDL_BUTTON_LEFT) {
            int mouseX = e.button.x;
            int mouseY = e.button.y;

            if (mouseX >= retryButton.rect.x && mouseX <= retryButton.rect.x + retryButton.rect.w &&
                mouseY >= retryButton.rect.y && mouseY <= retryButton.rect.y + retryButton.rect.h) {
                saveHighScore();
                return PLAYING;
            }

            if (mouseX >= menuButton.rect.x && mouseX <= menuButton.rect.x + menuButton.rect.w &&
                mouseY >= menuButton.rect.y && mouseY <= menuButton.rect.y + menuButton.rect.h) {
                saveHighScore();
                return MENU;
            }
        }
    }

    return GAME_OVER;
}

void Menu::toggleSound() {
    soundToggle.soundOn = !soundToggle.soundOn;

    if (soundToggle.soundOn) {
        Mix_ResumeMusic();
        Mix_VolumeMusic(10);
    }
    else {
        Mix_PauseMusic();
    }
}

void Menu::renderMainMenu(Graphics& graphics) {
    if (mainMenuBgTexture != NULL) {
        SDL_RenderCopy(renderer, mainMenuBgTexture, NULL, NULL);
    }

    for (const auto& button : mainMenuButtons) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &button.rect);

        SDL_Color textColor = { 0, 0, 0, 0 };
        int textX = button.rect.x + (button.rect.w - button.text.length() * 14) / 2;
        int textY = button.rect.y + (button.rect.h - 24) / 2;
        graphics.renderText(button.text, textX, textY, textColor);
    }

    SDL_Texture* currentSoundTexture = soundToggle.soundOn ? soundOnTexture : soundOffTexture;
    if (currentSoundTexture != NULL) {
        SDL_RenderCopy(renderer, currentSoundTexture, NULL, &soundToggle.rect);
    }

    SDL_Color titleColor = { 0, 0, 0, 0 };
    graphics.renderText("Du Cho Tan The", SCREEN_WIDTH / 2 - 70, 80, titleColor);
}

void Menu::renderGameOver(Graphics& graphics, int currentScore, int highScore) {
    if (gameOverBgTexture != NULL) {
        SDL_RenderCopy(renderer, gameOverBgTexture, NULL, NULL);
    }

    SDL_Color titleColor = { 255, 0, 0, 255 };
    graphics.renderText("GAME OVER", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 4, titleColor);

    SDL_Color scoreColor = { 255, 255, 255, 255 };
    std::string scoreText = "Your Score: " + std::to_string(currentScore);
    graphics.renderText(scoreText, SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 80, scoreColor);

    if (currentScore > highScore) highScore = currentScore;
    std::string highScoreText = "High Score: " + std::to_string(highScore);
    graphics.renderText(highScoreText, SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 40, scoreColor);

    SDL_SetRenderDrawColor(renderer, 60, 60, 140, 255);
    SDL_RenderFillRect(renderer, &retryButton.rect);

    int textX = retryButton.rect.x + (retryButton.rect.w - retryButton.text.length() * 14) / 2;
    int textY = retryButton.rect.y + (retryButton.rect.h - 24) / 2;
    graphics.renderText(retryButton.text, textX, textY, scoreColor);

    SDL_SetRenderDrawColor(renderer, 60, 60, 140, 255);
    SDL_RenderFillRect(renderer, &menuButton.rect);

    textX = menuButton.rect.x + (menuButton.rect.w - menuButton.text.length() * 14) / 2;
    textY = menuButton.rect.y + (menuButton.rect.h - 24) / 2;
    graphics.renderText(menuButton.text, textX, textY, scoreColor);
}

void Menu::renderHelp(Graphics& graphics) {
    if (helpBgTexture != NULL) {
        SDL_RenderCopy(renderer, helpBgTexture, NULL, NULL);
    }

    SDL_Color titleColor = { 255, 255, 100, 255 };
    graphics.renderText("HOW TO PLAY", SCREEN_WIDTH / 2 - 100, 80, titleColor);

    SDL_Color textColor = { 255, 255, 255, 255 };
    graphics.renderText("- Use Mouse/Space to jump", 100, 180, textColor);
    graphics.renderText("- Wall jump by touching walls", 100, 220, textColor);
    graphics.renderText("- Avoid obstacles to survive", 100, 260, textColor);
    graphics.renderText("- Cross maps to gain points", 100, 300, textColor);
    graphics.renderText("- Press ESC to return to menu", 100, 340, textColor);

    SDL_Color escColor = { 255, 100, 100, 255 };
    graphics.renderText("Press ESC to return to menu", SCREEN_WIDTH / 2 - 180, SCREEN_HEIGHT - 100, escColor);
}

void Menu::free() {
    if (mainMenuBgTexture != NULL) {
        SDL_DestroyTexture(mainMenuBgTexture);
        mainMenuBgTexture = NULL;
    }
    if (gameOverBgTexture != NULL) {
        SDL_DestroyTexture(gameOverBgTexture);
        gameOverBgTexture = NULL;
    }
    if (helpBgTexture != NULL) {
        SDL_DestroyTexture(helpBgTexture);
        helpBgTexture = NULL;
    }
    if (soundOnTexture != NULL) {
        SDL_DestroyTexture(soundOnTexture);
        soundOnTexture = NULL;
    }
    if (soundOffTexture != NULL) {
        SDL_DestroyTexture(soundOffTexture);
        soundOffTexture = NULL;
    }
}