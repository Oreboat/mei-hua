

#include "core/module.h"
#include "flecs.h"
#include "flecs/addons/flecs_c.h"
MODULE(sdl_m);


  typedef struct{
    
  }SDL_Window;

extern ECS_COMPONENT_DECLARE(SDL_Window);

void create_window(ecs_world_t *world, ecs_entity_t id);

int should_window_close(ecs_world_t *world, ecs_entity_t id);

void get_events();



