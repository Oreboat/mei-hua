#pragma once
#include "app.h"

typedef struct Module module_t;

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

#define MODULE(module_name) struct module_name{\
    module_t module;\
}
