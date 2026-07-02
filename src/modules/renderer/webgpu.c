#include "webgpu.h"
#include "core/app.h"
#include "flecs/addons/flecs_c.h"
#include "window/sdl.h"
#include "modules/renderer/renderer.h"

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
int wgpu_renderer_init(App *app, ecs_entity_t id){return 0;}
void wgpu_start_frame(){}
void wgpu_draw(){}
void wgpu_end_frame(){}
void on_adapter_request_ended(WGPURequestAdapterStatus status, WGPUAdapter adapter, WGPUStringView message, void *userdata1, void *userdata2){}

void on_device_request_ended(WGPURequestDeviceStatus status, WGPUDevice device, WGPUStringView message, void *userdata1, void *userdata2){}

WGPUAdapter get_physical_device(WGPURenderer *renderer){return NULL;}

void init_logical_device(WGPURenderer *renderer){}

void init_render_target(WGPURenderer *renderer){}

void init_pipeline(WGPURenderer *renderer){}
