#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <utility>
#include <vector>

#include "IAppProcess.h"

#pragma once

class App {
    protected:
        bool closeSignal = false;
        bool loaded = false;
        std::string tittle = "Snake";

        std::vector<IAppProcess*> processes;
        
        inline static App* app = nullptr;

        App(){}

    public:
        int kScreenWidth = 650;
        int kScreenHeight = 650;

        SDL_Window* window = nullptr;
        SDL_Surface* surface = nullptr;
        SDL_Renderer* renderer = nullptr;
        TTF_Font* generalFont = nullptr;

        // App(){

        // }

        void addProcess(IAppProcess* process){
            processes.push_back(process);
            if(loaded){
                process->init();
            }
        }

        std::pair<int, int> getScreenSize(){
            return {kScreenWidth, kScreenHeight};
        }

        void sendCloseSignal(){
            closeSignal = true;
        }

        bool getCloseSignal(){
            return closeSignal;
        }

        static App* get(){
            if(App::app == nullptr){
                App::app = new App();
            }

            return App::app;
        }

        bool init(){
            if (!SDL_Init(SDL_INIT_VIDEO)) {
                SDL_Log("SDL no pudo inicializarse: %s\n", SDL_GetError());
                return false;
            }

            if (!TTF_Init()) {
                SDL_Log("TTF no pudo inicializarse: %s\n", SDL_GetError());
                return false;
            }

            window = SDL_CreateWindow(tittle.c_str(), kScreenWidth, kScreenHeight, 0);
            if (window == nullptr) {
                SDL_Log("No se pudo crear la ventana: %s\n", SDL_GetError());
                return false;
            }

            renderer = SDL_CreateRenderer(window, NULL);
            if (renderer == nullptr) {
                SDL_Log("No se pudo crear el renderer: %s\n", SDL_GetError());
                return false;
            }

            generalFont = TTF_OpenFont("res/fonts/Roboto_Mono/static/RobotoMono-Regular.ttf", 18);
            if (generalFont == nullptr) {
                SDL_Log("TTF no pudo abrir el font: %s\n", SDL_GetError());
                return false;
            }

            if(!SDL_SetRenderVSync(renderer, 1)){
                SDL_Log("No se pudo activar el vsync: %s\n", SDL_GetError());
                return false;
            }

            long long timeCounter = 0;
            if(!SDL_GetCurrentTime(&timeCounter)){
                SDL_Log("No se pudo obtener el tiempo actual para la randomseed: %s\n", SDL_GetError());
                return false;
            }
            SDL_srand(timeCounter);

            for (IAppProcess* process : processes){
                if(!process->init()){
                    return false;
                }
            }

            loaded = true;
            return true;
        }

        void update(double* dt){
            // SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Update del juego");

            for (IAppProcess* process : processes){
                process->update(dt);
            }
        }

        void render(){
            SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00,  0x00);
            SDL_RenderClear(renderer);
            for (IAppProcess *process : processes){
                process->render(renderer);
            }
            SDL_RenderPresent(renderer);
        }

        void close(){
            // SDL_DestroySurface(screenSurface);
            for (IAppProcess* process : processes){
                process->close();
            }
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            window = nullptr;
            // screenSurface = nullptr;
            renderer = nullptr;
            SDL_Quit();
        }
};

App* app = App::get();