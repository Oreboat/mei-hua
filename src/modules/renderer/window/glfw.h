#pragma once

#include <flecs.h>
#include <stddef.h>
#include <GLFW/glfw3.h>
#include "core/module.h"



typedef struct{
    GLFWwindow *glfwWindow;
}Window;

MODULE(glfw_m);

extern ECS_COMPONENT_DECLARE(GlfwWindow);


void create_window(ecs_world_t *world, ecs_entity_t id);

void error_callback(int code, const char* description);

int should_window_close(ecs_world_t *world, ecs_entity_t id);

void get_events();