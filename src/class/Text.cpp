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

    Text(std::string text) {
        this->text = text;
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

    void render(SDL_Renderer* renderer) override {
        if(!visible) return;
        SDL_GetRenderDrawColor(renderer, &color.r, &color.g, &color.b, &color.a);
        SDL_RenderTextureRotated(renderer, texture, NULL, &posAndSize, degrees, NULL, SDL_FLIP_NONE);
    }

    void close() override {
        SDL_DestroyTexture(texture);
        delete this;
    }
};

} // namespace text