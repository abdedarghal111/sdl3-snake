#include <SDL3/SDL.h>

#pragma once

class IAppProcess {
public:
    virtual bool init() = 0;
    virtual void update(double* dt) = 0;
    virtual void render(SDL_Renderer* renderer) = 0;
    virtual void close() = 0;
};
