#include "core/app.h"
#ifndef RENDERER_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// #include <SDL3/SDL.h>
#include <webgpu/webgpu.h>
// #include "sdl3webgpu/sdl3webgpu.h"

#include "core/module.h"
#include "window/window.h"
MODULE(renderer_m);

void renderer_m_on_init(module_t *self, App *app){
    printf("Hello from the renderer module :3\n");
    add_module(app, window_m);
    ecs_entity_t renderer_entity = ecs_new(app->world);
    window_init(app->world, renderer_entity);
    while(window_should_close(app->world, renderer_entity)){
        poll_events();
    }
}

struct AppState;

struct {
    WGPUInstance wgpu_instance;
    WGPUSurface wgpu_surface;

    // Physical GPU handle
    WGPUAdapter wgpu_adapter;

    // Logical GPU handle
    WGPUDevice wgpu_device;

    // Command queue
    WGPUQueue wgpu_queue;

    WGPUCommandEncoder wgpu_command_encoder;
    WGPURenderPassEncoder wgpu_render_pass_encoder;

    WGPUColor clear_color;
} RendererState;


int initWebGPU() {
    // We create a descriptor
    WGPUInstanceDescriptor instance_desc = WGPU_INSTANCE_DESCRIPTOR_INIT;
    RendererState.wgpu_instance = wgpuCreateInstance(&instance_desc);

    assert(RendererState.wgpu_instance);

    //WGPUSurfaceDescriptor surface_desc = WGPU_SURFACE_DESCRIPTOR_INIT;
    // RendererState.wgpu_surface = SDL_GetWGPUSurface(RendererState.wgpu_instance, AppState.sdl_window); //wgpuInstanceCreateSurface(AppState.gpu_instance, &surface_desc);assert(surface != NULL);
    
    assert(RendererState.wgpu_surface);

    return EXIT_SUCCESS;
}

void onAdapterRequestEnded(WGPURequestAdapterStatus status, WGPUAdapter adapter, WGPUStringView message, void *userdata1, void *userdata2) {
    printf("request_adapter status=%#.8x message=%.*s\n", status,
           (int) message.length, message.data);
    
    RendererState.wgpu_adapter = adapter;
}

void onDeviceRequestEnded(WGPURequestDeviceStatus status, WGPUDevice device, WGPUStringView message, void *userdata1, void *userdata2) {
    printf("request_device status=%#.8x message=%.*s\n", status,
           (int) message.length, message.data);
    
    RendererState.wgpu_device = device;
}

WGPUAdapter getPhysicalDevice() {
    WGPURequestAdapterOptions options = WGPU_REQUEST_ADAPTER_OPTIONS_INIT;
    options.compatibleSurface = RendererState.wgpu_surface;

    WGPURequestAdapterCallbackInfo info = WGPU_REQUEST_ADAPTER_CALLBACK_INFO_INIT;
    info.callback = onAdapterRequestEnded;

    // The function returns the adapter immediately only on desktop. On web it is asyncronous, so we need to wait for the callback in that case
    wgpuInstanceRequestAdapter(RendererState.wgpu_instance, &options, info);

#ifdef __EMSCRIPTEN__
    while (!userData.requestEnded) {
        emscripten_sleep(100);
    }
#endif // __EMSCRIPTEN__


    return RendererState.wgpu_adapter;
}

void initLogicalDevice() {
    RendererState.wgpu_adapter = getPhysicalDevice();
    assert(RendererState.wgpu_adapter);

    WGPURequestDeviceCallbackInfo info = WGPU_REQUEST_DEVICE_CALLBACK_INFO_INIT;
    info.callback = onDeviceRequestEnded;

    wgpuAdapterRequestDevice(RendererState.wgpu_adapter, NULL, info);
    assert(RendererState.wgpu_device);

    RendererState.wgpu_queue = wgpuDeviceGetQueue(RendererState.wgpu_device);
    assert(RendererState.wgpu_queue);
}

void initRenderTarget() {
    WGPUSurfaceCapabilities surface_capabilities = {0};
    wgpuSurfaceGetCapabilities(RendererState.wgpu_surface, RendererState.wgpu_adapter, &surface_capabilities);

    WGPUSurfaceConfiguration config = WGPU_SURFACE_CONFIGURATION_INIT;
    config.device = RendererState.wgpu_device;
    config.usage = WGPUTextureUsage_RenderAttachment;
    config.format = surface_capabilities.formats[0];
    config.presentMode = WGPUPresentMode_Fifo;
    config.alphaMode = surface_capabilities.alphaModes[0];

    // Set surface size to the window size
    // SDL_GetWindowSize(AppState.sdl_window, &config.width, &config.height);

    wgpuSurfaceConfigure(RendererState.wgpu_surface, &config);

    assert(RendererState.wgpu_surface);
}

void initPipeline() {
    WGPUSurfaceCapabilities surface_capabilities = {0};
    // wgpuSurfaceGetCapabilities(RendererState.wgpu_surface, RendererState.wgpu_surface, &surface_capabilities);

    WGPUShaderModule shader_module; // = frmwrk_load_shader_module(RendererState.wgpu_device, "shader.wgsl");
    assert(shader_module);

    WGPUPipelineLayout pipeline_layout = wgpuDeviceCreatePipelineLayout(
        RendererState.wgpu_device, &(const WGPUPipelineLayoutDescriptor){
                        .label = {"pipeline_layout", WGPU_STRLEN},
                    });
    assert(pipeline_layout);

    WGPURenderPipeline render_pipeline = wgpuDeviceCreateRenderPipeline(
        RendererState.wgpu_device,
        &(const WGPURenderPipelineDescriptor){
            .label = {"render_pipeline", WGPU_STRLEN},
            .layout = pipeline_layout,
            .vertex =
                (const WGPUVertexState){
                    .module = shader_module,
                    .entryPoint = {"vs_main", WGPU_STRLEN},
                },
            .fragment =
                &(const WGPUFragmentState){
                    .module = shader_module,
                    .entryPoint = {"fs_main", WGPU_STRLEN},
                    .targetCount = 1,
                    .targets =
                        (const WGPUColorTargetState[]){
                            (const WGPUColorTargetState){
                                .format = surface_capabilities.formats[0],
                                .writeMask = WGPUColorWriteMask_All,
                            },
                        },
                },
            .primitive =
                (const WGPUPrimitiveState){
                    .topology = WGPUPrimitiveTopology_TriangleList,
                },
            .multisample =
                (const WGPUMultisampleState){
                    .count = 1,
                    .mask = 0xFFFFFFFF,
                },
        });
    assert(render_pipeline);
}

void rendererInit() {
    initWebGPU();
    initLogicalDevice();
    initRenderTarget();
    //initPipeline();
}

void rendererCleanup() {
    wgpuSurfaceUnconfigure(RendererState.wgpu_surface);
    wgpuSurfaceRelease(RendererState.wgpu_surface);
    wgpuInstanceRelease(RendererState.wgpu_instance);
}

void rendererClear() {

}

void rendererStartFrame() {

}

void rendererEndFrame() {

}

void rendererDraw() {

    // WGPUSurfaceConfiguration wgpu_surface_configuration = {};

    // WGPUTextureViewDescriptor view_desc = {};
    // view_desc.format = wgpu_surface_configuration.format;
    // view_desc.dimension = WGPUTextureViewDimension_2D;
    // view_desc.mipLevelCount = WGPU_MIP_LEVEL_COUNT_UNDEFINED;
    // view_desc.arrayLayerCount = WGPU_ARRAY_LAYER_COUNT_UNDEFINED;
    // view_desc.aspect = WGPUTextureAspect_All;


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


    WGPUSurfaceTexture surface_texture = WGPU_SURFACE_TEXTURE_INIT;
    wgpuSurfaceGetCurrentTexture(RendererState.wgpu_surface, &surface_texture);


    //assert(RendererState.wgpu_surface.s)

    //assert(surface_texture);
    //assert(surface_texture.status != WGPUSurfaceGetCurrentTextureStatus_SuccessOptimal);

    if(!surface_texture.texture) { 
        // This can mean the surface is occluded because the window is minimized.
        return;
    }
    
    assert(surface_texture.texture);

    WGPUTextureView frame = wgpuTextureCreateView(surface_texture.texture, NULL);
    assert(frame);

    //return;
    
    WGPUCommandEncoderDescriptor encoderDesc = WGPU_COMMAND_ENCODER_DESCRIPTOR_INIT;
    encoderDesc.nextInChain = NULL;
    //encoderDesc.label = "My command encoder";
    RendererState.wgpu_command_encoder = wgpuDeviceCreateCommandEncoder(RendererState.wgpu_device, &encoderDesc);

    wgpuCommandEncoderInsertDebugMarker(RendererState.wgpu_command_encoder, (WGPUStringView){"Do one thing", WGPU_STRLEN});
    wgpuCommandEncoderInsertDebugMarker(RendererState.wgpu_command_encoder, (WGPUStringView){"Do another thing"});


    RendererState.wgpu_render_pass_encoder =
        wgpuCommandEncoderBeginRenderPass(
            RendererState.wgpu_command_encoder,
            &(const WGPURenderPassDescriptor){
                .label = {"render_pass_encoder", WGPU_STRLEN},
                .colorAttachmentCount = 1,
                .colorAttachments =
                    (const WGPURenderPassColorAttachment[]){
                        (const WGPURenderPassColorAttachment){
                            .view = frame,
                            .loadOp = WGPULoadOp_Clear,
                            .storeOp = WGPUStoreOp_Store,
                            .depthSlice = WGPU_DEPTH_SLICE_UNDEFINED,
                            .clearValue = RendererState.clear_color,
                        },
                    },
            });
    
    assert(RendererState.wgpu_render_pass_encoder);

    wgpuRenderPassEncoderEnd(RendererState.wgpu_render_pass_encoder);
    wgpuRenderPassEncoderRelease(RendererState.wgpu_render_pass_encoder);

    WGPUCommandBuffer command_buffer = wgpuCommandEncoderFinish(
        RendererState.wgpu_command_encoder, &(const WGPUCommandBufferDescriptor){
                             .label = {"command_buffer", WGPU_STRLEN},
            });
    wgpuCommandEncoderRelease(RendererState.wgpu_command_encoder);
    
    assert(command_buffer);

    // Send the commands we just recorded to the GPU
    wgpuQueueSubmit(RendererState.wgpu_queue, 1, (const WGPUCommandBuffer[]){command_buffer});
}

void rendererPresent() {
    wgpuSurfacePresent(RendererState.wgpu_surface);
}

#endif /* RENDERER_H */