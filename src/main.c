#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <SDL3/SDL.h>
#include <webgpu/webgpu.h>

#include "core/app.h"
#include "sdl3webgpu/sdl3webgpu.h"
//#include "sdl3webgpu/sdl3webgpu.c"

struct {
    SDL_Window* sdl_window;
    WGPUInstance wgpu_instance;
    WGPUSurface wgpu_surface;
    WGPUDevice wgpu_device;
} AppState;

#include "modules/renderer/renderer.h"

int init() {
    printf("initializing sdl %d.%d.%d\n",
       SDL_MAJOR_VERSION,
       SDL_MINOR_VERSION,
       SDL_MICRO_VERSION);
    if(!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO |SDL_INIT_EVENTS | SDL_INIT_GAMEPAD)){
        printf("{Error 0001} SDL_Init failed: %s\n", SDL_GetError());
        printf("current driver: %s\n", SDL_GetCurrentVideoDriver());
        return EXIT_FAILURE;
    }
    printf("current driver: %s\n", SDL_GetCurrentVideoDriver());
    int width = 640;
    int height = 360;

	int flags = 0;
    flags |= SDL_WINDOW_HIGH_PIXEL_DENSITY;

    AppState.sdl_window = SDL_CreateWindow("mei-hua!", width, height, flags);
    if(AppState.sdl_window == NULL) {
        printf("{Error 0002} SDL Window is null");
        return EXIT_FAILURE;
    }
    SDL_ShowWindow(AppState.sdl_window);
    rendererInit();
    
    return EXIT_SUCCESS;
}

int cleanup() {
    rendererCleanup();

	SDL_DestroyWindow(AppState.sdl_window);
    SDL_Quit();

    return EXIT_SUCCESS;
}

double game_time = 0.0;
void mainLoop(double delta_time) {
    RendererState.clear_color = (WGPUColor){.r = SDL_sin(game_time) * 0.5 + 0.5, .g = SDL_sin(game_time + 1.0) * 0.5 + 0.5, .b = SDL_sin(game_time + 2.0) * 0.5 + 0.5};
    
    game_time += delta_time;

    // Graphics
    rendererClear();
    rendererDraw();
    rendererPresent();
}

int main(int argc, char * argv[]) {
    App *app = app_init();
    add_module(app, renderer);
    printf("Hello, World\n");
    printf("drivers:\n");
    for (int i = 0; i < SDL_GetNumVideoDrivers(); i++) {
        printf("  %s\n", SDL_GetVideoDriver(i));
    }
    
    
    if(init() == EXIT_FAILURE){
        return EXIT_FAILURE;
    }
    //Main loop flag
    bool running = true;

    //Event handler
    SDL_Event e;
    
    //While application is running
    while( running )
    {
        //Handle events on queue
        while( SDL_PollEvent( &e ) != 0 )
        {
            //User requests quit
            if( e.type == SDL_EVENT_QUIT )
            {
                running = false;
            }
        }

        mainLoop(1.0 / 60.0);
    }

    cleanup();

    return EXIT_SUCCESS;
}