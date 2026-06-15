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
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS | SDL_INIT_GAMEPAD);
    
    int width = 640;
    int height = 360;

	int flags = 0;
    flags |= SDL_WINDOW_HIGH_PIXEL_DENSITY;

    AppState.sdl_window = SDL_CreateWindow("Meowser!", width, height, flags);

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

void mainLoop(double delta_time) {
    rendererClear();
    rendererDraw();
    rendererPresent();
}

int main(int argc, char * argv[]) {
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