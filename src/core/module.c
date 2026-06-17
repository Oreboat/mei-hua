#include "module.h"
#include <stdlib.h>

struct Module* create_module_list(int list_size){
    return calloc(list_size, sizeof(struct Module));
}