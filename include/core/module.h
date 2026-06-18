#pragma once
#include "app.h"



struct Module{
    //the name of the module
    char *name;
    //when the module is intialized
    void(*on_init)(module_t *self, App *app);
    //run when module setup is complete
    void(*on_finish)(module_t *self, App *app);
    //run when deinitializing the module
    void(*deinit)(module_t *self, App *app);
};

struct Module *create_module_list(int list_size);

#define MODULE(module_name)typedef struct module_name{\
    module_t module;\
}module_name;\
void module_name##_on_init(module_t *self, App *app)
