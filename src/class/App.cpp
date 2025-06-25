#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

class App {
    protected:
        bool closeSignal = false;
        
        inline static App* app = nullptr;

        App(){}

    public:
        int kScreenWidth = 640;
        int kScreenHeight = 480;

        SDL_Window* window = nullptr;
        SDL_Renderer* renderer = nullptr;
        // SDL_Surface* screenSurface = nullptr;

        // App(){

        // }

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

            window = SDL_CreateWindow("SDL3 Tutorial: Hello SDL3", kScreenWidth, kScreenHeight, 0);
            if (window == nullptr) {
                SDL_Log("No se pudo crear la ventana: %s\n", SDL_GetError());
                return false;
            }

            renderer = SDL_CreateRenderer(window, NULL);
            if (renderer == nullptr) {
                SDL_Log("No se pudo crear el renderer: %s\n", SDL_GetError());
                return false;
            }
            
            // screenSurface = SDL_GetWindowSurface(window);
            // if (screenSurface == nullptr) {
            //     SDL_Log("No se pudo obtener la superficie de la ventana: %s\n", SDL_GetError());
            //     return false;
            // }

            if(!SDL_SetRenderVSync(renderer, 1)){
                SDL_Log("No se pudo activar el vsync: %s\n", SDL_GetError());
                return false;
            }

            return true;
        }

        void update(float* dt){
            // SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Update del juego");
        }

        void render(){
        }

        void close(){
            // SDL_DestroySurface(screenSurface);
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            window = nullptr;
            // screenSurface = nullptr;
            renderer = nullptr;
            SDL_Quit();
        }
};

App* app = App::get();