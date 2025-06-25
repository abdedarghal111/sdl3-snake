#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>

#include "IAppProcess.h"
#include "App.cpp"

#pragma once

namespace snake {

int GRIDS = 20;
int TILE_SIZE = 0;
std::string direction = "right";

TTF_Font *font = nullptr;

App* app = App::get();

class Snake : public IAppProcess {
public:
    Snake() {
        app->addProcess(this);
    }

    bool init() override {
        if (!TTF_Init()) {
            SDL_Log("TTF no pudo inicializarse: %s\n", SDL_GetError());
            return false;
        }

        font = TTF_OpenFont("res/fonts/Roboto_Mono/static/RobotoMono-Regular.ttf", 18);
        if (!font) {
            SDL_Log("TTF no pudo abrir el font: %s\n", SDL_GetError());
            return false;
        }

        TILE_SIZE = app->getScreenSize().first / GRIDS;
        return true;
    }
    
    void update(float* dt) override {

        //check keys
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
    }

    void render(SDL_Renderer* renderer) override {
        // renderer
    }

    void close() override {
        TTF_CloseFont(font);
        font = nullptr;
        delete this;
    }
};

// create snake
Snake* snake = new Snake();

} // namespace snake