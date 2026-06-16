#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <SDL3/SDL.h>
#include <webgpu/webgpu.h>

#include "sdl3webgpu/sdl3webgpu.h"
//#include "sdl3webgpu/sdl3webgpu.c"

struct {
    SDL_Window* sdl_window;
    WGPUInstance wgpu_instance;
    WGPUSurface wgpu_surface;
    WGPUDevice wgpu_device;
} AppState;

#include "renderer/renderer.h"

int init() {
    printf("initializing sdl\n");
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS | SDL_INIT_GAMEPAD);
    
    
    int width = 640;
    int height = 360;

	int flags = 0;
    flags |= SDL_WINDOW_HIGH_PIXEL_DENSITY;

    AppState.sdl_window = SDL_CreateWindow("mei-hua!", width, height, flags);
    
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
    printf("Hello, World\n");
    
    init();
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