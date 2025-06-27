#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <string>

#include "class/App.cpp"
#include "class/Snake.cpp"
#include "unique/FPSRecorder.cpp"

int FPS = 60;
unsigned long long S_IN_NS = 1'000'000'000ll;

unsigned long long MAX_NS_DELAY = S_IN_NS / FPS;

int main(int argc, char* argv[]) {

    if(!app->init()) {
        SDL_Log("No se pudo inicializar el programa!\n");
        app->close();
        return 1;
    }

    SDL_SetLogPriorities(SDL_LOG_PRIORITY_VERBOSE);

    // SDL_SetWindowResizable(gWindow, true);
    // SDL_SetWindowMaximumSize(gWindow, 0, 0);

    // SDL_SetWindowBordered(app->window, false);
    SDL_Event e;
    SDL_zero(e);

    double deltaTime = 0;
    unsigned long long nFrameStart = 0;
    unsigned long long nFrameEnd = 0;
    unsigned long long nsFrameTime = 0;
    while (!app->getCloseSignal()) {
        nFrameStart = SDL_GetTicksNS();
        while (SDL_PollEvent(&e) == true) {
            if (e.type == SDL_EVENT_QUIT) {
                app->sendCloseSignal();
            }
        }

        app->update(&deltaTime);
        app->render();

        nFrameEnd = SDL_GetTicksNS();
        nsFrameTime = nFrameEnd - nFrameStart;
        if (nsFrameTime < MAX_NS_DELAY) {
            SDL_DelayNS(MAX_NS_DELAY - nsFrameTime);
        }

        // accuraced stable fps
        // if (nsFrameTime < MAX_NS_DELAY) {
        //     SDL_DelayNS(MAX_NS_DELAY - nsFrameTime - 10'000'000ll);
        // }
        // while(SDL_GetTicksNS() - nFrameStart < MAX_NS_DELAY){}
        deltaTime = (double)(SDL_GetTicksNS() - nFrameStart) / S_IN_NS;
        fpsRecorder->record(1 / deltaTime);
    }

    app->close();
    return 0;
}