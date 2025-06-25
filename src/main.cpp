#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <string>

#include "class/App.cpp"
#include "class/Snake.cpp"

uint8_t FPS = 60;
Uint64 F_MAX_DELAY = 1'000'000ull / FPS;

int main(int argc, char* argv[]) {


    if(!app->init()) {
        SDL_Log("No se pudo inicializar el programa!\n");
        app->close();
        return 1;
    }

    SDL_SetLogPriorities(SDL_LOG_PRIORITY_VERBOSE);

    // SDL_SetWindowResizable(gWindow, true);
    // SDL_SetWindowMaximumSize(gWindow, 0, 0);

    SDL_Event e;
    SDL_zero(e);

    float deltaTime = F_MAX_DELAY / 1'000'000.0f;
    Uint64 nFrameStart = 0;
    Uint64 nFrameEnd = 0;
    Uint64 nFrameTime = 0;
    while (!app->getCloseSignal()) {
        nFrameStart = SDL_GetTicksNS();
        while (SDL_PollEvent(&e) == true) {
            if (e.type == SDL_EVENT_QUIT) {
                app->sendCloseSignal();
            }
        }

        // SDL_FillSurfaceRect(
        //     gScreenSurface, nullptr,
        //     SDL_MapSurfaceRGB(gScreenSurface, 0xFF, 0xFF, 0xFF)
        // );
        // SDL_UpdateWindowSurface(gWindow);

        app->update(&deltaTime);
        app->render();

        nFrameEnd = SDL_GetTicksNS();
        nFrameTime = nFrameEnd - nFrameStart;
        if (nFrameTime < F_MAX_DELAY) {
            SDL_DelayNS(F_MAX_DELAY - nFrameTime);
        }
        deltaTime = (SDL_GetTicksNS() - nFrameStart) / 1'000'000.0f;
    }

    app->close();
    return 0;
}