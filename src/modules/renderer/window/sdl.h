#pragma once

#include "SDL3/SDL_video.h"
#include "core/module.h"
#include "flecs.h"
#include "flecs/addons/flecs_c.h"
MODULE(sdl_m);


typedef struct{
  SDL_Window *window;   
}Window;

extern ECS_COMPONENT_DECLARE(Window);

int sdl_create_window(App *app, ecs_entity_t id);

int sdl_should_window_close(App *app, ecs_entity_t id);

void sdl_cleanup(App *app, ecs_entity_t id);

void get_events();



