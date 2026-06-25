#pragma once

#include "core/module.h"
#include "flecs/addons/flecs_c.h"
#include <flecs.h>

MODULE(window_m);

extern ECS_COMPONENT_DECLARE(World);

typedef struct{
  int (*window_init)(App *app, ecs_entity_t id);
  int (*window_should_close)(App *app, ecs_entity_t id);
  void (*window_cleanup)(App *app, ecs_entity_t id);
}WindowInterface;


extern ECS_COMPONENT_DECLARE(WindowInterface);
