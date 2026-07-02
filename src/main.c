#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "modules/default_modules.h"

#include "core/app.h"
// #include "sdl3webgpu/sdl3webgpu.h"
//#include "sdl3webgpu/sdl3webgpu.c"



//#include "modules/renderer/renderer.h"

int init() {
    // printf("initializing sdl %d.%d.%d\n",
    //    SDL_MAJOR_VERSION,
    //    SDL_MINOR_VERSION,
    //    SDL_MICRO_VERSION);
    // if(!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO |SDL_INIT_EVENTS | SDL_INIT_GAMEPAD)){
    //     printf("{Error 0001} SDL_Init failed: %s\n", SDL_GetError());
    //     printf("current driver: %s\n", SDL_GetCurrentVideoDriver());
    //     return EXIT_FAILURE;
    // }
    // printf("current driver: %s\n", SDL_GetCurrentVideoDriver());
    // int width = 640;
    // int height = 360;

	// int flags = 0;
    // flags |= SDL_WINDOW_HIGH_PIXEL_DENSITY;

    // AppState.sdl_window = SDL_CreateWindow("mei-hua!", width, height, flags);
    // if(AppState.sdl_window == NULL) {
    //     printf("{Error 0002} SDL Window is null");
    //     return EXIT_FAILURE;
    // }
    // SDL_ShowWindow(AppState.sdl_window);
    // rendererInit();
    
    return EXIT_SUCCESS;
}


int main(int argc, char * argv[]) {
    App *app = app_init();
    add_module(app, default_m);
    printf("Hello, World\n");
    return EXIT_SUCCESS;
}
