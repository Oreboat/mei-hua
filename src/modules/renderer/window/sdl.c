#include "sdl.h"
#include "SDL3/SDL.h"
#include "SDL3/SDL_init.h"
#include "flecs/addons/flecs_c.h"
#include <stdio.h>

ECS_COMPONENT_DECLARE(Window);
void sdl_m_on_init(module_t *self, App *app){
  SDL_InitFlags flags = SDL_INIT_VIDEO | SDL_INIT_AUDIO |SDL_INIT_EVENTS | SDL_INIT_GAMEPAD;
  if(!SDL_Init(flags)){ 
    //terminate the module once I add that feature
    return;
  }
  //add SDL component
  ECS_COMPONENT_DEFINE(app->world, Window);
}

void create_window(ecs_world_t *world, ecs_entity_t id){
  ecs_set(world, id, Window, {
      .window = SDL_CreateWindow("mei-hua", 1280, 720, NULL)
  });

  const Window *window = ecs_get(world, id, Window);
  if(window->window == NULL){
    printf("Window is Null");
  }
}

int should_window_close(ecs_world_t *world, ecs_entity_t id){
  return 0;
}

void get_events(){

}
