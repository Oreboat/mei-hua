#pragma once

#include "core/module.h"
#include <flecs.h>

MODULE(window_m);

void window_init(ecs_world_t *world, ecs_entity_t id);

int window_should_close(ecs_world_t *world, ecs_entity_t id);

void poll_events();