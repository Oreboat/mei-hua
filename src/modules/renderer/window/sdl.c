#include "sdl.h"
#include "SDL3/SDL.h"
#include "SDL3/SDL_init.h"
#include "flecs.h"
#include "flecs/addons/flecs_c.h"
#include "flecs/private/api_defines.h"
#include "window.h"
#include <stdio.h>
#include <stdlib.h>

ECS_COMPONENT_DECLARE(Window);
void sdl_m_on_init(module_t *self, App *app){
  SDL_InitFlags flags = SDL_INIT_VIDEO | SDL_INIT_AUDIO |SDL_INIT_EVENTS | SDL_INIT_GAMEPAD;
  if(!SDL_Init(flags)){ 
    //terminate the module once I add that feature
    return;
  }
  //add SDL component
  ECS_COMPONENT_DEFINE(app->world, Window);
  ecs_singleton_set(app->world, WindowInterface, {
      .window_init = create_window,
      .window_should_close = should_window_close,
      .window_cleanup = cleanup,
      });
}

int create_window(App *app, ecs_entity_t id){
  ecs_set(app->world, id, Window, {
      .window = SDL_CreateWindow("mei-hua", 1280, 720, NULL)
  });
  const Window *window = ecs_get(app->world, id, Window);
  if(window->window == NULL){
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int should_window_close(App *app, ecs_entity_t id){
  return 0;
}

void cleanup(App *app, ecs_entity_t id){
  Window *win = ecs_get_mut_id(app->world, id, ecs_id(Window)); 
} 
