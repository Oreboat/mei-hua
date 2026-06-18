#include "glfw.h"
#include "GLFW/glfw3.h"
#include "core/app.h"
#include <flecs/addons/flecs_c.h>
#include <stdio.h>

ECS_COMPONENT_DECLARE(Window);


void glfw_m_on_init(module_t *self, App *app){
    ECS_COMPONENT_DEFINE(app->world, Window);
    glfwSetErrorCallback(error_callback);
    if(!glfwInit()){
        char *description;
        int code = glfwGetError(*description);
        printf("GLFW error {%d}: %s\n", code, description);
    }
}

void error_callback(int code, const char* description)
{
    printf("GLFW error {%d}: %s\n", code, description);
}


void create_window(ecs_world_t *world, ecs_entity_t id){
    ecs_set(world, id, Window, {
        .glfwWindow = glfwCreateWindow(1280, 720, "mei hua", NULL, NULL)
    });
    const Window *window = ecs_get(world, id, Window);
    printf("window value is %d\n", window->glfwWindow != NULL);
}

int should_window_close(ecs_world_t *world, ecs_entity_t id){
    if(ecs_get(world, id, Window)->glfwWindow == NULL){
        printf("GLFW window is null");
    };
    return glfwWindowShouldClose(ecs_get(world, id, Window)->glfwWindow);
}

void get_events(){
    glfwPollEvents();
}