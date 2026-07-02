#pragma once

#include "flecs.h"
#include "flecs/addons/flecs_c.h"
typedef struct {}MH_Start;

typedef struct {}MH_FixedPreUpdate;
typedef struct {}MH_FixedUpdate;
typedef struct {}MH_FixedPostUpdate;

typedef struct {}MH_PreUpdate;
typedef struct {}MH_Update;
typedef struct {}MH_PostUpdate;
typedef struct {}MH_Render;

//reassign pipelines here to have custom stages
typedef struct {
  ecs_entity_t start_pipeline;
  ecs_entity_t fixed_update_pipeline;
  ecs_entity_t update_pipeline;
}MH_Pipelines;

extern ECS_COMPONENT_DECLARE(MH_Pipelines);
