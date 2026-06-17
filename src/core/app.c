#include "app.h"
#include "module.h"
#include <stdlib.h>
#include <webgpu/webgpu.h>

#define APP_MODULE_ALLOC_SIZE 10


App* app_init(){
    App *app = (App*)malloc(sizeof(App));
    app->world = ecs_init();
    app->modules = create_module_list(APP_MODULE_ALLOC_SIZE); //this may be changed
    app->module_count = 0;
    app->module_current_max = APP_MODULE_ALLOC_SIZE;
    return app;
}

void app_deinit(App *self){
    //loop through modules
    free(self);
}

void app_add_module(App *self, module_t module){
    if(self->modules == NULL) return;
    if(self->module_count == self->module_current_max){
        self->modules = realloc(self->modules,sizeof(struct Module) + 10);
        if(self->modules == NULL) return;
        self->module_current_max += 10;
    }
}