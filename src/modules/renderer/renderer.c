#include "renderer.h"
#include "flecs/addons/flecs_c.h"


ECS_COMPONENT_DECLARE(RendererInterface);

void renderer_m_on_init(module_t *self, App *app){
    ECS_COMPONENT_DEFINE(app->world, RendererInterface);
}

