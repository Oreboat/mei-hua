#include "webgpu.h"
#include "core/app.h"
#include "flecs/addons/flecs_c.h"
#include "window/sdl.h"
#include "renderer.h"

ECS_COMPONENT_DECLARE(WGPURenderer);


void wgpu_m_on_init(module_t *Self, App *app){
  
  ECS_COMPONENT_DEFINE(app->world, WGPURenderer);

  add_module(app, sdl_m);
  add_module(app, renderer_m);
  ecs_singleton_set(app->world, RendererInterface, {
    ._init = wgpu_renderer_init,
    ._start_frame = wgpu_start_frame,
    ._draw = wgpu_draw,
    ._end_frame = wgpu_end_frame,
    .render_query = ecs_query(app->world, {
        .terms = {
          {.id = ecs_id(WGPURenderer)},
          {.id = ecs_id(Window)},
        }
        })
  });
}
