#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <string>

#ifdef __EMSCRIPTEN__
    #include <emscripten.h>
#endif

#include "class/App.h"
#include "class/Snake.h"
#include "unique/FPSRecorder.h"

int FPS = 60;
unsigned long long S_IN_NS = 1'000'000'000ll;

unsigned long long MAX_NS_DELAY = S_IN_NS / FPS;
SDL_Event e;

double deltaTime = 0;
unsigned long long nFrameStart = 0;
unsigned long long nFrameEnd = 0;
unsigned long long nsFrameTime = 0;

void mainLoop() {
    #ifndef __EMSCRIPTEN__
        nFrameStart = SDL_GetTicksNS();
    #endif
    while (SDL_PollEvent(&e) == true) {
        if (e.type == SDL_EVENT_QUIT) {
            app->sendCloseSignal();
        }
    }

    app->update(&deltaTime);
    app->render();

    #ifndef __EMSCRIPTEN__
        nFrameEnd = SDL_GetTicksNS();
        nsFrameTime = nFrameEnd - nFrameStart;
        if (nsFrameTime < MAX_NS_DELAY) {
            SDL_DelayPrecise(MAX_NS_DELAY - nsFrameTime);
        }

        deltaTime = (double)(SDL_GetTicksNS() - nFrameStart) / S_IN_NS;
        fpsRecorder->record(1 / deltaTime);
    #else
        if(app->getCloseSignal()) {
            app->close();
            emscripten_cancel_main_loop();
            return;
        }
        deltaTime = (double)(SDL_GetTicksNS() - nFrameStart) / S_IN_NS;
        fpsRecorder->record(1 / deltaTime);
        nFrameStart = SDL_GetTicksNS();
    #endif
}

int main(int argc, char* argv[]) {

    if(!app->init()) {
        SDL_Log("No se pudo inicializar el programa!\n");
        app->close();
        return 1;
    }

    SDL_zero(e);
    SDL_SetLogPriorities(SDL_LOG_PRIORITY_VERBOSE);
    
    #ifdef __EMSCRIPTEN__
        emscripten_set_main_loop(mainLoop, 60, 1);
    #else
        while (!app->getCloseSignal()) {
            mainLoop();
        }
    #endif

    app->close();
    return 0;
}