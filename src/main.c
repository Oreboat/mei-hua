#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <SDL3/SDL.h>
#include <webgpu/webgpu.h>

#include "sdl3webgpu/sdl3webgpu.h"
//#include "sdl3webgpu/sdl3webgpu.c"

struct {
    SDL_Window* window;
    WGPUInstance gpu_instance;
} AppState;

int initWebGPU() {
    // We create a descriptor
    WGPUInstanceDescriptor instance_desc = WGPU_INSTANCE_DESCRIPTOR_INIT;
    AppState.gpu_instance = wgpuCreateInstance(&instance_desc);

    assert(AppState.gpu_instance != NULL);

    WGPUSurfaceDescriptor surface_desc = WGPU_SURFACE_DESCRIPTOR_INIT;
    //WGPUSurface* surface = wgpuInstanceCreateSurface(AppState.gpu_instance, &surface_desc);assert(surface != NULL);
    
    return EXIT_SUCCESS;
}

int init() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS | SDL_INIT_GAMEPAD);
    
    int width = 640;
    int height = 360;

	int flags = 0;
    flags |= SDL_WINDOW_HIGH_PIXEL_DENSITY;

    AppState.window = SDL_CreateWindow("Meowser!", width, height, flags);

    SDL_ShowWindow(AppState.window);

    initWebGPU();
    
    return EXIT_SUCCESS;
}

int cleanup() {
    wgpuInstanceRelease(AppState.gpu_instance);

    SDL_Quit();

    return EXIT_SUCCESS;
}

int main(int argc, char * argv[]) {
    init();

    //Main loop flag
    bool quit = false;

    //Event handler
    SDL_Event e;

    //While application is running
    while( !quit )
    {
        //Handle events on queue
        while( SDL_PollEvent( &e ) != 0 )
        {
            //User requests quit
            if( e.type == SDL_EVENT_QUIT )
            {
                quit = true;
            }
        }
    }

    cleanup();

    return EXIT_SUCCESS;
}