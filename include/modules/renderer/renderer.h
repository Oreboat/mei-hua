#pragma once

#include "core/app.h"
#include "flecs.h"
#include "flecs/addons/flecs_c.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// #include <SDL3/SDL.h>
#include <webgpu/webgpu.h>
// #include "sdl3webgpu/sdl3webgpu.h"

#include "core/module.h"
MODULE(renderer_m);

ECS_COMPONENT_DECLARE(Renderer);

typedef struct{
  //init(app, id)
  int (*_init)(App *app, ecs_entity_t id);
  //start frame
  void (*_start_frame)();
  //render
  void (*_draw)();
  //end frame
  void (*_end_frame)();
  //query for render components
  ecs_query_t render_query;
}RendererInterface;

extern ECS_COMPONENT_DECLARE(RendererInterface);

int initWebGPU();

void onAdapterRequestEnded(WGPURequestAdapterStatus status, WGPUAdapter adapter, WGPUStringView message, void *userdata1, void *userdata2);

void onDeviceRequestEnded(WGPURequestDeviceStatus status, WGPUDevice device, WGPUStringView message, void *userdata1, void *userdata2);

WGPUAdapter getPhysicalDevice();

void initLogicalDevice();

void initRenderTarget();

void initPipeline();

void rendererInit();

void rendererCleanup();

void rendererClear();

void rendererStartFrame();

void rendererEndFrame();

void rendererDraw();

void rendererPresent();