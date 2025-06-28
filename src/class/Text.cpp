#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>

#include "IAppProcess.h"
#include "App.cpp"

#pragma once

namespace text {

App* app = App::get();

class Text : public IAppProcess {
public:

    SDL_Texture* texture = nullptr;
    std::string text = "";
    SDL_Color color = {255, 255, 255, 255};
    float aspectRatio = 1.0f;
    SDL_FRect posAndSize = {0, 0, 0, 0};
    double degrees = 0.0f;
    bool visible = true;
    bool autoDraw = true;
    bool hasShadow = false;

    Text(std::string text, bool autoDraw = true, bool hasShadow = false) {
        this->text = text;
        this->autoDraw = autoDraw;
        this->hasShadow = hasShadow;
        app->addProcess(this);
    }

    void setText(std::string text) {
        this->text = text;
        if(texture != nullptr){
            SDL_DestroyTexture(texture);
        }
        app->surface = TTF_RenderText_Blended(app->generalFont, text.c_str(), 0,{255, 255, 255, 255});
        texture = SDL_CreateTextureFromSurface(app->renderer, app->surface);
        SDL_DestroySurface(app->surface);
        posAndSize.h = (float)texture->h;
        posAndSize.w = (float)texture->w;
    }

    bool init() override {
        app->surface = TTF_RenderText_Blended(app->generalFont, text.c_str(), 0,{255, 255, 255, 255});
        texture = SDL_CreateTextureFromSurface(app->renderer, app->surface);
        SDL_DestroySurface(app->surface);
        posAndSize.h = (float)texture->h;
        posAndSize.w = (float)texture->w;
        return true;
    }
    
    void update(double* dt) override {
    }

    void manualRender(SDL_Renderer* renderer) {
        if(!visible) return;

        if(hasShadow){
            // previous color
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 100);
            SDL_RenderFillRect(renderer, &posAndSize);

        }

        SDL_SetTextureColorMod(texture, color.r, color.g, color.b);
        SDL_SetTextureAlphaMod(texture, color.a);
        SDL_RenderTextureRotated(renderer, texture, NULL, &posAndSize, degrees, NULL, SDL_FLIP_NONE);
    }

    void render(SDL_Renderer* renderer) override {
        if(!autoDraw) return;
        manualRender(renderer);
    }

    void close() override {
        SDL_DestroyTexture(texture);
        delete this;
    }
};

} // namespace text