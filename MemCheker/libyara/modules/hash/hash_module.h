#pragma once
#include "../../include/yara/modules.h"

#define MODULE_NAME hash

extern int module_initialize(YR_MODULE* module);
extern int module_finalize(YR_MODULE* module);
extern int module_load(
    YR_SCAN_CONTEXT* context,
    YR_OBJECT* module_object,
    void* module_data,
    size_t module_data_size);

extern int module_declarations(YR_OBJECT* module);
extern int module_unload(YR_OBJECT* module_object);