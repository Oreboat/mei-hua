#include "window.h"
#include <flecs.h>
#include <flecs/addons/flecs_c.h>
#include "core/app.h"
#include "glfw.h"



void window_m_on_init(module_t *self, App *app){
    add_module(app, glfw_m);
}

void window_init(ecs_world_t *world, ecs_entity_t id){
    create_window(world, id);
}

int window_should_close(ecs_world_t *world, ecs_entity_t id){
    return should_window_close(world, id);
}

void poll_events(){
    get_events();
}