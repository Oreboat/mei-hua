//core files
#include "core/app.h"
#include "core/module.h"
#include "core/schedules.h"
//library files
#include <flecs.h>
#include <flecs/addons/flecs_c.h>
#include <flecs/private/addons.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "flecs/private/api_defines.h"
//module definitions, these will probably end up either replaced as the engine develops or moved to core
#include "modules/renderer/window/window.h"
#include "modules/renderer/renderer.h"

#define APP_MODULE_ALLOC_SIZE 10

ECS_COMPONENT_DECLARE(MH_Pipelines);


App* app_init(){
    App *app = (App*)malloc(sizeof(App));
    app->world = ecs_init();
    app->modules = create_module_list(APP_MODULE_ALLOC_SIZE); //this may be changed
    app->module_count = 0;
    app->module_current_max = APP_MODULE_ALLOC_SIZE;
    ECS_TAG(app->world, MH_Start);
    ECS_TAG(app->world, MH_FixedPreUpdate);
    ECS_TAG(app->world, MH_FixedUpdate);
    ECS_TAG(app->world, MH_FixedPostUpdate);
    ECS_TAG(app->world, MH_PreUpdate);
    ECS_TAG(app->world, MH_Update);
    ECS_TAG(app->world, MH_PostUpdate);
    ECS_TAG(app->world, MH_Render);
    ecs_add_id(app->world, ecs_id(MH_Pipelines), EcsSingleton);

    ecs_singleton_set(app->world, MH_Pipelines, {
        .start_pipeline = ecs_pipeline_init(app->world, &(ecs_pipeline_desc_t){
            .query.terms = {
              {.id = EcsSystem},
              {.id = MH_Start},
            }
            }),
        .fixed_update_pipeline = ecs_pipeline_init(app->world, &(ecs_pipeline_desc_t){
            .query.terms = {
              {.id = EcsSystem},
              {.id = MH_FixedPreUpdate},
              {.id = MH_FixedUpdate},
              {.id = MH_FixedPostUpdate}
            }
            }),
        .update_pipeline = ecs_pipeline_init(app->world, &(ecs_pipeline_desc_t){
            .query.terms = {
              {.id = EcsSystem},
              {.id = MH_PreUpdate},
              {.id = MH_Update},
              {.id = MH_PostUpdate},
            }
            }),
        });
    const MH_Pipelines *pipelines = ecs_get(app->world, ecs_id(MH_Pipelines), MH_Pipelines);
    ecs_set_pipeline(app->world, pipelines->start_pipeline);
    return app;
}

void app_deinit(App *self){
    //loop through modules
    free(self);
}

void app_add_module(App *self, module_t *module){
    if(self->modules == NULL) return;
    if(module->on_init == NULL) return;
    
    if(app_has_module(self, module)) return;
    if(self->module_count == self->module_current_max){
        self->modules = realloc(self->modules,sizeof(struct Module) * (self->module_current_max + APP_MODULE_ALLOC_SIZE));
        if(self->modules == NULL) return;
        self->module_current_max += 10;
    }
    
    self->modules[self->module_count] = *module; 
    printf("adding Module %s\n", self->modules[self->module_count].name);
    self->module_count += 1;
    self->modules[self->module_count - 1].on_init(&self->modules[self->module_count - 1], self);
    
}

bool app_has_module(App *self, module_t *module){
    for(int i = 0; i < self->module_count; i++){
        
        if(strcmp(self->modules[i].name, module->name) == 0){
            printf("Module %s already exists\n", module->name);
            return true;
        }
    }
    return false;
}

//the main loop of the engine
void app_run(App *self){
  ecs_entity_t renderer_entity = ecs_new(self->world);
  const RendererInterface *r_interface = ecs_get(self->world, ecs_id(RendererInterface), RendererInterface);
  const WindowInterface *w_interface = ecs_get(self->world, ecs_id(WindowInterface), WindowInterface);
  const MH_Pipelines *pipelines = ecs_get(self->world, ecs_id(MH_Pipelines), MH_Pipelines);

  
  if(r_interface != NULL && r_interface->_init != NULL) r_interface->_init(self, renderer_entity);
  if(w_interface != NULL && w_interface->_init != NULL) w_interface->_init(self, renderer_entity);
  //loop while should not close
  if(w_interface != NULL && w_interface->_should_close != NULL){
    //start the main loop
    ecs_set_pipeline(self->world, pipelines->start_pipeline);
    ecs_progress(self->world, 0.0f);
    while (w_interface->_should_close(self, renderer_entity) == 0) {
      
      //set pipeline as fixed update
      //progress fixed update pipeline

      //set pipeline as update
      ecs_set_pipeline(self->world, pipelines->update_pipeline);
      ecs_progress(self->world, 0.0);
      //progress update pipeline
      
    }
  }
}


