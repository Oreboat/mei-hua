#include "core/app.h"
#include "core/module.h"
#include "webgpu/webgpu.h"
#include <webgpu/wgpu.h>

MODULE(wgpu_m);

typedef struct{
  WGPUInstance instance;
  WGPUSurface surface;
  //physical GPU handle
  WGPUAdapter adapter;
  //logical GPU handle
  WGPUDevice device;
  //command queue
  WGPUQueue queue;
  WGPUCommandEncoder command_encoder;
  WGPURenderPassEncoder render_pass_encoder;
  WGPUColor clear_color;
}WGPURenderer;

//renderer api

int wgpu_renderer_init(App *app, ecs_entity_t id);

void wgpu_start_frame();

void wgpu_draw();

void wgpu_end_frame();

//wgpu functions
void on_adapter_request_ended(WGPURequestAdapterStatus status, WGPUAdapter adapter, WGPUStringView message, void *userdata1, void *userdata2);

void on_device_request_ended(WGPURequestDeviceStatus status, WGPUDevice device, WGPUStringView message, void *userdata1, void *userdata2);

WGPUAdapter get_physical_device(WGPURenderer *renderer);

void init_logical_device(WGPURenderer *renderer);

void init_render_target(WGPURenderer *renderer);

void init_pipeline(WGPURenderer *renderer);


