

#include "modules/default_modules.h"
#include "core/app.h"
#include "renderer/webgpu.h"
void default_m_on_init(module_t *self, App *app){
  //for later, #ifdef window
  add_module(app, wgpu_m);
}
