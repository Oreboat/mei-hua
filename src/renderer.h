#ifndef RENDERER_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <SDL3/SDL.h>
#include <webgpu/webgpu.h>
#include "sdl3webgpu/sdl3webgpu.h"

struct AppState;

struct {
    WGPUInstance wgpu_instance;
    WGPUSurface wgpu_surface;
    WGPUDevice wgpu_device;
} RendererState;


int initWebGPU() {
    // We create a descriptor
    WGPUInstanceDescriptor instance_desc = WGPU_INSTANCE_DESCRIPTOR_INIT;
    RendererState.wgpu_instance = wgpuCreateInstance(&instance_desc);

    assert(RendererState.wgpu_instance != NULL);

    WGPUSurfaceDescriptor surface_desc = WGPU_SURFACE_DESCRIPTOR_INIT;
    RendererState.wgpu_surface = SDL_GetWGPUSurface(RendererState.wgpu_instance, AppState.sdl_window); //wgpuInstanceCreateSurface(AppState.gpu_instance, &surface_desc);assert(surface != NULL);
    
    return EXIT_SUCCESS;
}

void rendererInit() {
    initWebGPU();
}

void rendererCleanup() {
    wgpuSurfaceRelease(RendererState.wgpu_surface);
    wgpuInstanceRelease(RendererState.wgpu_instance);
}

void rendererDraw() {
    WGPUSurfaceConfiguration wgpu_surface_configuration = {};

    WGPUTextureViewDescriptor view_desc = {};
    view_desc.format = wgpu_surface_configuration.format;
    view_desc.dimension = WGPUTextureViewDimension_2D;
    view_desc.mipLevelCount = WGPU_MIP_LEVEL_COUNT_UNDEFINED;
    view_desc.arrayLayerCount = WGPU_ARRAY_LAYER_COUNT_UNDEFINED;
    view_desc.aspect = WGPUTextureAspect_All;

    // WGPUTextureView texture_view = wgpuTextureCreateView(surface_texture.texture, &view_desc);

    // WGPURenderPassColorAttachment color_attachments = {};
    // color_attachments.depthSlice = WGPU_DEPTH_SLICE_UNDEFINED;
    // color_attachments.loadOp = WGPULoadOp_Clear;
    // color_attachments.storeOp = WGPUStoreOp_Store;
    // color_attachments.clearValue = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
    // color_attachments.view = texture_view;
    
    // WGPURenderPassDescriptor render_pass_desc = WGPU_RENDER_PASS_DESCRIPTOR_INIT;
    // render_pass_desc.colorAttachmentCount = 1;
    // render_pass_desc.colorAttachments = &color_attachments;
    // render_pass_desc.depthStencilAttachment = NULL;

    // WGPUCommandEncoderDescriptor enc_desc = {};
    // WGPUCommandEncoder encoder = wgpuDeviceCreateCommandEncoder(RendererState.wgpu_device, &enc_desc);

    // WGPURenderPassEncoder pass = wgpuCommandEncoderBeginRenderPass(encoder, &render_pass_desc);
}

void rendererPresent() {
    wgpuSurfacePresent(RendererState.wgpu_surface);
}

#endif /* RENDERER_H */