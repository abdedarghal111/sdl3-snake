#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>

#include "IAppProcess.h"
#include "App.cpp"
#include "class/Text.cpp"
#include "unique/FPSRecorder.cpp"

#pragma once

namespace snake {

int GRIDS = 20;
int TILE_SIZE = 0;
std::string direction = "right";

text::Text* helloWorldText = new text::Text("I use arch by the way");
long long a = 0;
text::Text* count = new text::Text(std::to_string(a));

App* app = App::get();

class Snake : public IAppProcess {
public:
    Snake() {
        app->addProcess(this);
    }

    bool init() override {
        TILE_SIZE = app->getScreenSize().first / GRIDS;
        count->posAndSize.y = helloWorldText->posAndSize.h;
        
        return true;
    }
    
    void update(double* dt) override {

        //check keys
        count->setText(std::to_string( fpsRecorder->getAVG() ).substr(0, 4));

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
        
        
        // SDL_FillSurfaceRect(
        //     gScreenSurface, nullptr,
        //     SDL_MapSurfaceRGB(gScreenSurface, 0xFF, 0xFF, 0xFF)
        // );
        // SDL_UpdateWindowSurface(gWindow);
    }

    void close() override {
        delete this;
    }
};

// create snake
Snake* snake = new Snake();

} // namespace snake