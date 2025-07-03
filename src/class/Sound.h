#pragma once

#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <string>

#include "IAppProcess.h"
#include "App.h"


namespace sound {

class Sound : public IAppProcess {
public:

    std::string soundFile = "";
    Mix_Chunk* soundInstance = nullptr;
    int volume = 0;
    double duration = 0;
    int channel = -1;
    bool playing = false;
    long long timeTracker = 0;

    Sound(std::string soundFile) {
        this->soundFile = soundFile;
        app->addProcess(this);
    }

    bool play() {
        if(playing) {
            Mix_HaltChannel(channel);
        }
        channel = Mix_PlayChannel(-1, soundInstance, 0);

        playing = true;
        timeTracker = SDL_GetTicks();
        return channel != -1;
    }

    bool init() override {
        soundInstance = Mix_LoadWAV(soundFile.c_str());
        if(soundInstance == nullptr) {
            printf("Failed to load beat music! SDL_mixer Error: %s\n", SDL_GetError());
            return false;
        }
        return true;
    }
    
    void update(double* dt) override {
        if(duration == 0) {
            return;
        }
        if(playing) {
            if(SDL_GetTicks() - timeTracker > duration * 1000) {
                Mix_HaltChannel(channel);
                playing = false;
            }
        }
    }

    void render(SDL_Renderer* renderer) override {}

    void close() override {
        Mix_FreeChunk(soundInstance);
        delete this;
    }
};

} // namespace sound