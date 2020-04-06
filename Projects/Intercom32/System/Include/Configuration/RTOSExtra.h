#pragma once

#include "freertos/FreeRTOS.h"

#define configWIFISVC_STACK_DEPTH (1024 * 3)
#define configHTTPSVC_STACK_DEPTH (1024 * 2)
#define configGATEWAYSVC_STACK_DEPTH (1024 * 2)
#define configFWUPDATESVC_STACK_DEPTH (1024 * 10)

#define configTOTAL_PROJECT_HEAP_SIZE_ALLOCATED (configWIFISVC_STACK_DEPTH + \
                                                 configHTTPSVC_STACK_DEPTH + \
                                                 configGATEWAYSVC_STACK_DEPTH + \
                                                 configFWUPDATESVC_STACK_DEPTH )
