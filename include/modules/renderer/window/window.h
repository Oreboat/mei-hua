#pragma once

#include "core/module.h"
#include "flecs/addons/flecs_c.h"
#include <flecs.h>

MODULE(window_m);

extern ECS_COMPONENT_DECLARE(World);

typedef struct{
  int (*_init)(App *app, ecs_entity_t id);
  int (*_should_close)(App *app, ecs_entity_t id);
  void (*_cleanup)(App *app, ecs_entity_t id);
}WindowInterface;


extern ECS_COMPONENT_DECLARE(WindowInterface);
