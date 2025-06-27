#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include <vector>

#include "IAppProcess.h"
#include "App.cpp"
#include "class/Text.cpp"
#include "unique/FPSRecorder.cpp"

#pragma once

namespace snake {

float JUMPS_PER_SEC = 5; // saltos por segundo (velocidad)
int GRIDS = 20;
int TILE_SIZE = 0;
unsigned long long timeTracker = SDL_GetTicks();
std::vector<SDL_FRect> snakeTiles;

std::string direction = "right";
bool addTile = false;

text::Text* fpsText = new text::Text("FPS:");
text::Text* avgFPS = new text::Text(std::to_string(fpsRecorder->getAVG()).substr(0, 6));

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
        TILE_SIZE = app->getScreenSize().first / GRIDS;
        avgFPS->posAndSize.x = fpsText->posAndSize.w;

        int startXPoint = app->getScreenSize().first / 2;
        int startYPoint = app->getScreenSize().second / 2;
        snakeTiles.push_back({(float)startYPoint, (float)startXPoint, (float)TILE_SIZE, (float)TILE_SIZE});
        
        return true;
    }
    
    void update(double* dt) override {

        //check keys
        avgFPS->setText(std::to_string(fpsRecorder->getAVG()).substr(0, 6));

        const bool* keyStates = SDL_GetKeyboardState(NULL);
        if (keyStates[SDL_SCANCODE_D]){
            direction = "right";
        }else if(keyStates[SDL_SCANCODE_W]){
            direction = "up";
        }else if(keyStates[SDL_SCANCODE_A]){
            direction = "left";
        }else if(keyStates[SDL_SCANCODE_S]){
            direction = "down";
        }

        if(SDL_GetTicks() - timeTracker > 1 / JUMPS_PER_SEC * 1000) {
            if(snakeTiles.size() < 6){
                addTile = true;
            }
            float lastX = snakeTiles[snakeTiles.size() - 1].x;
            float lastY = snakeTiles[snakeTiles.size() - 1].y;
            
            timeTracker = SDL_GetTicks();
            if (direction == "right") {
                snakeTiles[0].x += TILE_SIZE;
                snakeTiles[0].y += 0;
                moveAllTiles();
            }else if (direction == "left") {
                snakeTiles[0].x -= TILE_SIZE;
                snakeTiles[0].y += 0;
                moveAllTiles();
            }else if (direction == "up") {
                snakeTiles[0].x += 0;
                snakeTiles[0].y -= TILE_SIZE;
                moveAllTiles();
            }else if (direction == "down") {
                snakeTiles[0].x += 0;
                snakeTiles[0].y += TILE_SIZE;
                moveAllTiles();
            }

            if(addTile) {
                snakeTiles.push_back({lastX, lastY, (float)TILE_SIZE, (float)TILE_SIZE});
                addTile = false;
            }
        }
    }

    void render(SDL_Renderer* renderer) override {
        // renderer
        
        for (SDL_FRect rect : snakeTiles) {
            SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
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