#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"

/* Called by configASSERT() when an assertion fails. */
void vAssertCalled(const char *const pcFileName, unsigned long ulLine)
{
    fprintf(stderr, "ASSERT FAILED: %s:%lu\n", pcFileName, ulLine);
    fflush(stderr);

    taskDISABLE_INTERRUPTS();
    for (;;) {
    }
}