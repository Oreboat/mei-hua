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

void app_add_module(App *self, module_t module);

#define ADD_MODULE(app, module_name) 