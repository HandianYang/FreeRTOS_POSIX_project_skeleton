#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"

#include "app.h"

/*
 * Production entry point. Keeps only the boot sequence: hand control to the
 * application, then start the scheduler. Demos are built separately through
 * demo/runner.c and never share this file.
 */
int main(void)
{
    if (app_start() != pdPASS) {
        fprintf(stderr, "Failed to start application\n");
        return 1;
    }

    vTaskStartScheduler();

    fprintf(stderr, "Scheduler stopped unexpectedly\n");
    return 1;
}
