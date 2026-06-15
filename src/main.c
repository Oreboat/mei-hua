#include <stdio.h>
#include <stdlib.h>

#include <SDL3/SDL.h>
#include <webgpu/webgpu.h>

int main(int argc, char * argv[]) {
    /* my first program in C */
    printf("Hello World!\n");

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS | SDL_INIT_GAMEPAD);

    int width = 640;
    int height = 360;

	int flags = 0;
    flags |= SDL_WINDOW_HIGH_PIXEL_DENSITY;

    SDL_Window* window = SDL_CreateWindow("Meowser!", width, height, flags);

    SDL_ShowWindow(window);



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

    return EXIT_SUCCESS;
}