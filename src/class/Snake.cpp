#include <SDL3/SDL.h>
#include <string>
#include <vector>

#include "IAppProcess.h"
#include "App.cpp"
#include "class/Text.cpp"
#include "unique/FPSRecorder.cpp"

#pragma once

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
std::vector<SDL_FRect> snakeTiles;
std::vector<SDL_FRect> appleTiles;

std::string direction = "right";
std::string prevDirection = "right";
bool addTile = false;
bool alive = true;
int randTile = SDL_rand(20) + 1;

text::Text* fpsText = new text::Text("FPS:");
text::Text* avgFPSText = new text::Text(std::to_string(fpsRecorder->getAVG()).substr(0, 6));
text::Text* snakeLenText = new text::Text("Snake size:");
text::Text* snakeLengthText = new text::Text(std::to_string(1));

App* app = App::get();

void moveAllTiles() {
    // mueve el último al penultimo y para antes de alcanzar el primero de todos
    for (size_t i = snakeTiles.size() - 1; i > 0; i--) {
        snakeTiles[i].x = snakeTiles[i - 1].x;
        snakeTiles[i].y = snakeTiles[i - 1].y;
    }
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
        
        fpsText->posAndSize.x = (float)TILES_PADDING + 3;
        fpsText->posAndSize.y = (float)TILES_PADDING;
        avgFPSText->posAndSize.y = fpsText->posAndSize.y;
        avgFPSText->posAndSize.x = fpsText->posAndSize.x + fpsText->posAndSize.w;
        snakeLenText->posAndSize.y = avgFPSText->posAndSize.y + avgFPSText->posAndSize.h;
        snakeLenText->posAndSize.x = fpsText->posAndSize.x;
        snakeLengthText->posAndSize.y = snakeLenText->posAndSize.y;
        snakeLengthText->posAndSize.x = snakeLenText->posAndSize.x + snakeLenText->posAndSize.w;

        int startXPoint = TILE_SIZE/2 * TILES + TILES_PADDING;
        int startYPoint = startXPoint;
        snakeTiles.push_back({(float)startYPoint, (float)startXPoint, (float)TILE_SIZE, (float)TILE_SIZE});
        
        return true;
    }
    
    void update(double* dt) override {
        //check input
        const bool* keyStates = SDL_GetKeyboardState(NULL);
        if (keyStates[SDL_SCANCODE_D] && prevDirection != "left") {
            direction = "right";
        }else if(keyStates[SDL_SCANCODE_W] && prevDirection != "down"){
            direction = "up";
        }else if(keyStates[SDL_SCANCODE_A] && prevDirection != "right"){
            direction = "left";
        }else if(keyStates[SDL_SCANCODE_S] && prevDirection != "up"){
            direction = "down";
        }

        // if game over
        if(!alive){
            return;
        }

        // TODO: borrar
        if(snakeTiles.size() < 6){
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

            if(addTile) {
                snakeTiles.push_back({lastX, lastY, (float)TILE_SIZE, (float)TILE_SIZE});
                addTile = false;
            }
        }

        // check collision
        for (SDL_FRect rect : snakeTiles) {
            if (rect.x == app->getScreenSize().first - TILE_SIZE) {
                alive = false;
            }else if (rect.x == 0) {
                alive = false;
            }else if (rect.y == app->getScreenSize().second - TILE_SIZE) {
                alive = false;
            }else if (rect.y == 0) {
                alive = false;
            }
        }

        // check apple existance
        if(appleTiles.size() < MAX_APPLES_ALIVE && SDL_GetTicks() - appleTimeTracker > SPAWN_APPLES_DELAY) {
            
        }

        // show stats
        avgFPSText->setText(std::to_string(fpsRecorder->getAVG()).substr(0, 6));
        snakeLengthText->setText(std::to_string(snakeTiles.size()));
    }

    void render(SDL_Renderer* renderer) override {
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        // draw playZone rectangle
        SDL_FRect playZoneRect = {(float)TILES_PADDING, (float)TILES_PADDING, (float)W_TILES, (float)W_TILES};
        SDL_RenderRect(renderer, &playZoneRect);
        
        // draw snake
        for (SDL_FRect rect : snakeTiles) {
            // SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
            SDL_RenderFillRect(renderer, &rect);
        }
    }

    void close() override {
        delete this;
    }
};

// create snake
Snake* snake = new Snake();

} // namespace snake