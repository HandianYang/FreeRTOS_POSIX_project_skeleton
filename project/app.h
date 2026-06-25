#ifndef APP_H
#define APP_H

#include "FreeRTOS.h"

/* Create the application's initial tasks. Called from main() before the
 * scheduler is started. Return pdPASS on success, pdFAIL otherwise. */
BaseType_t app_start(void);

#endif /* APP_H */
