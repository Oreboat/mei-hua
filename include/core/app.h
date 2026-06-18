#pragma once
#include "flecs.h"
#include <stddef.h>


typedef struct Module module_t;

typedef struct{
    
    //modules
    module_t *modules;
    size_t module_current_max;
    int module_count;
    
    ecs_world_t *world;
}App;

App* app_init();

void app_deinit(App *app);

void app_add_module(App *self, module_t *module);

bool app_has_module(App *self, module_t *module);

void app_run(App *self);
//this is just what flecs does, but uses the app
#define add_system(app, system, phase, ...) ECS_SYSTEM(app.world, system, phase, __VA_ARGS__)

#define add_module(app, module_name) module_name module_name##_module;\
module_name##_module.module.on_init = module_name##_on_init;\
module_name##_module.module.name = #module_name;\
app_add_module(app, (module_t*) &module_name##_module)
