#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"

#include "demo.h"

/*
 * Generic entry point for demo builds. Shared by every demo: the Makefile
 * selects which demo module (providing demo_start()) gets linked in.
 */
int main(void)
{
    if (demo_start() != pdPASS) {
        fprintf(stderr, "Failed to start demo\n");
        return 1;
    }

    vTaskStartScheduler();

    fprintf(stderr, "Scheduler stopped unexpectedly\n");
    return 1;
}
