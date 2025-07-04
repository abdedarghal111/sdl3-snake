#pragma once

#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <string>
#include <vector>

#include "IAppProcess.h"
#include "App.h"
#include "Text.h"
#include "Sound.h"
#include "../unique/FPSRecorder.h"


namespace snake {

int MAX_APPLES_ALIVE = 10;
int SPAWN_APPLES_DELAY = 2 * 1000;
float JUMPS_PER_SEC = 5; // saltos por segundo (velocidad)
int TILES = 20;
int TILE_SIZE = 0;
int W_TILES = 0;
int TILES_PADDING = 0;
unsigned long long movementTimeTracker = SDL_GetTicks();
unsigned long long appleTimeTracker = SDL_GetTicks();
unsigned long long blinkingTimer = SDL_GetTicks();
std::vector<SDL_FRect> snakeTiles;
std::vector<SDL_FRect> appleTiles;

std::string direction = "right";
std::string prevDirection = "right";
bool addTile = false;
bool alive = true;

text::Text* fpsText = new text::Text("FPS:", false, true);
text::Text* avgFPSText = new text::Text(std::to_string(fpsRecorder->getAVG()).substr(0, 6), false, true);
text::Text* snakeLenText = new text::Text("Snake size:", false, true);
text::Text* snakeLengthText = new text::Text(std::to_string(1), false, true);
text::Text* appleText = new text::Text("Apples in map:", false, true);
text::Text* appleCountText = new text::Text(std::to_string(appleTiles.size()), false, true);
text::Text* restartText = new text::Text("Press 'space' to restart", false, true);
text::Text* windowNotFocusedText = new text::Text("Click on this windows to play", false, true);

sound::Sound* eatSound = new sound::Sound("res/sounds/eat.mp3");
sound::Sound* dieSound = new sound::Sound("res/sounds/gameOver.mp3");

void moveAllTiles() {
    // mueve el último al penultimo y para antes de alcanzar el primero de todos
    for (size_t i = snakeTiles.size() - 1; i > 0; i--) {
        snakeTiles[i].x = snakeTiles[i - 1].x;
        snakeTiles[i].y = snakeTiles[i - 1].y;
    }
}

int isTouchingElementOfList(std::vector<SDL_FRect> list, float x, float y) {
    for (size_t i = 0; i < list.size(); i++) {
        if (list[i].x == x && list[i].y == y) {
            return (int)i;
        }
    }
    return -1;
}

class Snake : public IAppProcess {
public:
    Snake() {
        app->addProcess(this);
    }

    bool init() override {

        TILE_SIZE = app->getScreenSize().first / TILES;
        W_TILES = TILES * TILE_SIZE;
        TILES_PADDING = (app->getScreenSize().first - W_TILES) / 2;
        
        fpsText->posAndSize.x = (float)TILES_PADDING + 1;
        fpsText->posAndSize.y = (float)TILES_PADDING + 1;
        avgFPSText->posAndSize.y = fpsText->posAndSize.y;
        avgFPSText->posAndSize.x = fpsText->posAndSize.x + fpsText->posAndSize.w;
        snakeLenText->posAndSize.y = avgFPSText->posAndSize.y + avgFPSText->posAndSize.h;
        snakeLenText->posAndSize.x = fpsText->posAndSize.x;
        snakeLengthText->posAndSize.y = snakeLenText->posAndSize.y;
        snakeLengthText->posAndSize.x = snakeLenText->posAndSize.x + snakeLenText->posAndSize.w;
        appleText->posAndSize.y = snakeLenText->posAndSize.y + snakeLenText->posAndSize.h;
        appleText->posAndSize.x = fpsText->posAndSize.x;
        appleCountText->posAndSize.y = appleText->posAndSize.y;
        appleCountText->posAndSize.x = appleText->posAndSize.x + appleText->posAndSize.w;

        restartText->posAndSize.h *= 1.3f;
        restartText->posAndSize.w *= 1.3f;
        restartText->posAndSize.y = app->getScreenSize().first/2 - restartText->posAndSize.h/2;
        restartText->posAndSize.x = app->getScreenSize().first/2 - restartText->posAndSize.w/2;
        restartText->visible = false;

        windowNotFocusedText->posAndSize.h *= 1.3f;
        windowNotFocusedText->posAndSize.w *= 1.3f;
        windowNotFocusedText->posAndSize.y = app->getScreenSize().first/2 - windowNotFocusedText->posAndSize.h/2;
        windowNotFocusedText->posAndSize.x = app->getScreenSize().first/2 - windowNotFocusedText->posAndSize.w/2;
        windowNotFocusedText->visible = false;

        int startXPoint = TILE_SIZE/2 * TILES + TILES_PADDING;
        int startYPoint = startXPoint;
        snakeTiles.push_back({(float)startYPoint, (float)startXPoint, (float)TILE_SIZE, (float)TILE_SIZE});

        eatSound->duration = 0.4;
        
        return true;
    }
    
    void update(double* dt) override {
        //check input
        const bool* keyStates = SDL_GetKeyboardState(NULL);
        if ((keyStates[SDL_SCANCODE_D] || keyStates[SDL_SCANCODE_RIGHT]) && prevDirection != "left") {
            direction = "right";
        }else if((keyStates[SDL_SCANCODE_W] || keyStates[SDL_SCANCODE_UP]) && prevDirection != "down"){
            direction = "up";
        }else if((keyStates[SDL_SCANCODE_A] || keyStates[SDL_SCANCODE_LEFT]) && prevDirection != "right"){
            direction = "left";
        }else if((keyStates[SDL_SCANCODE_S] || keyStates[SDL_SCANCODE_DOWN]) && prevDirection != "up"){
            direction = "down";
        }

        // show stats
        avgFPSText->setText(std::to_string(fpsRecorder->getAVG()).substr(0, 6));
        snakeLengthText->setText(std::to_string(snakeTiles.size()));
        appleCountText->setText(std::to_string(appleTiles.size()));
        windowNotFocusedText->visible = !app->isFocused();

        // if game over
        if(!alive){
            if( SDL_GetTicks() - blinkingTimer > 700){
                restartText->visible = !restartText->visible;
                blinkingTimer = SDL_GetTicks();
            }
            if(keyStates[SDL_SCANCODE_SPACE]){
                // reset
                restartText->visible = false;
                alive = true;
                snakeTiles.clear();
                appleTiles.clear();
                movementTimeTracker = SDL_GetTicks();
                appleTimeTracker = SDL_GetTicks();
                direction = "right";
                prevDirection = "right";
                int startXPoint = TILE_SIZE/2 * TILES + TILES_PADDING;
                int startYPoint = startXPoint;
                snakeTiles.push_back({(float)startYPoint, (float)startXPoint, (float)TILE_SIZE, (float)TILE_SIZE});
            }
            return;
        }

        // tamaño inicial
        if(snakeTiles.size() < 3){
            addTile = true;
        }

        // move snake and add tile if needed
        if(SDL_GetTicks() - movementTimeTracker > 1 / JUMPS_PER_SEC * 1000) {
            float lastX = snakeTiles[snakeTiles.size() - 1].x;
            float lastY = snakeTiles[snakeTiles.size() - 1].y;
            movementTimeTracker = SDL_GetTicks();
            prevDirection = direction;

            if (direction == "right") {
                moveAllTiles();
                snakeTiles[0].x += TILE_SIZE;
                if(snakeTiles[0].x >= (TILE_SIZE * TILES) + TILES_PADDING) {
                    snakeTiles[0].x = (float)TILES_PADDING;
                }
            }else if (direction == "left") {
                moveAllTiles();
                snakeTiles[0].x -= TILE_SIZE;
                if(snakeTiles[0].x < TILES_PADDING) {
                    snakeTiles[0].x = (float)(TILE_SIZE * TILES) - TILE_SIZE + TILES_PADDING;
                }
            }else if (direction == "up") {
                moveAllTiles();
                snakeTiles[0].y -= TILE_SIZE;
                if(snakeTiles[0].y < TILES_PADDING) {
                    snakeTiles[0].y = (float)(TILE_SIZE * TILES) - TILE_SIZE + TILES_PADDING;
                }
            }else if (direction == "down") {
                moveAllTiles();
                snakeTiles[0].y += TILE_SIZE;
                if(snakeTiles[0].y >= (TILE_SIZE * TILES) + TILES_PADDING) {
                    snakeTiles[0].y = (float)TILES_PADDING;
                }
            }

            // snake ate apple
            int touchingApple = isTouchingElementOfList(appleTiles, snakeTiles[0].x, snakeTiles[0].y);
            if(touchingApple != -1) {
                addTile = true;
                appleTiles.erase(appleTiles.begin() + touchingApple);
                eatSound->play();
            }

            if(addTile) {
                snakeTiles.push_back({lastX, lastY, (float)TILE_SIZE, (float)TILE_SIZE});
                addTile = false;
            }
        }

        // check collision
        for (size_t i = 1; i < snakeTiles.size(); i++) {
            if (snakeTiles[i].x == snakeTiles[0].x && snakeTiles[i].y == snakeTiles[0].y) {
                alive = false;
                restartText->visible = true;
                blinkingTimer = SDL_GetTicks();
                dieSound->play();
            }
        }

        // check apple existance
        if(appleTiles.size() < MAX_APPLES_ALIVE && SDL_GetTicks() - appleTimeTracker > SPAWN_APPLES_DELAY) {
            
            size_t touchingSnake = 0;
            size_t touchingApple = 0;
            int randX = 0;
            int randY = 0;

            do {
                randX = (SDL_rand(20) * TILE_SIZE) + TILES_PADDING; // 0-19
                randY = (SDL_rand(20) * TILE_SIZE) + TILES_PADDING; // 0-19

                touchingSnake = isTouchingElementOfList(snakeTiles, (float)randX, (float)randY);
                touchingApple = isTouchingElementOfList(appleTiles, (float)randX, (float)randY);
            }while(touchingSnake != -1 || touchingApple != -1);

            appleTiles.push_back({(float)randX, (float)randY, (float)TILE_SIZE, (float)TILE_SIZE});
            appleTimeTracker = SDL_GetTicks();
        }
    }

    void render(SDL_Renderer* renderer) override {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        // draw playZone rectangle
        
        // draw snake
        for (SDL_FRect rect : snakeTiles) {
            SDL_RenderFillRect(renderer, &rect);
        }
        
        // draw apple
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        for (SDL_FRect rect : appleTiles) {
            SDL_RenderFillRect(renderer, &rect);
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_FRect playZoneRect = {(float)TILES_PADDING, (float)TILES_PADDING, (float)W_TILES, (float)W_TILES};
        SDL_RenderRect(renderer, &playZoneRect);

        // draw text
        fpsText->manualRender(renderer);
        avgFPSText->manualRender(renderer);
        snakeLenText->manualRender(renderer);
        snakeLengthText->manualRender(renderer);
        appleText->manualRender(renderer);
        appleCountText->manualRender(renderer);

        restartText->manualRender(renderer);
        windowNotFocusedText->manualRender(renderer);
    }

    void close() override {
        delete this;
    }
};

// create snake
Snake* snake = new Snake();

} // namespace snake