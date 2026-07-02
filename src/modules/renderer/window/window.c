#include "modules/renderer/window/window.h"
#include <flecs.h>
#include <flecs/addons/flecs_c.h>
#include "core/app.h"
#include "sdl.h"

ECS_COMPONENT_DECLARE(WindowInterface);

void window_m_on_init(module_t *self, App *app){
  ECS_COMPONENT_DEFINE(app->world, WindowInterface);
  ECS_COMPONENT_DEFINE(app->world, Window);

  ecs_add_id(app->world, ecs_id(WindowInterface), EcsSingleton);
}
