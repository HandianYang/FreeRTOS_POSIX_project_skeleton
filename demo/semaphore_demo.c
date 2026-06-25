#include "demo.h"

#include <stdio.h>

#include "task.h"
#include "semphr.h"

static SemaphoreHandle_t event_semaphore;
static SemaphoreHandle_t console_mutex;

static void safe_print(const char *message)
{
    if (xSemaphoreTake(
            console_mutex,
            pdMS_TO_TICKS(100)) == pdTRUE) {

        fputs(message, stdout);
        fflush(stdout);

        xSemaphoreGive(console_mutex);
    }
}

static void producer_task(void *parameters)
{
    (void) parameters;

    for (;;) {
        vTaskDelay(pdMS_TO_TICKS(1000));

        safe_print("Producer: event generated\n");

        xSemaphoreGive(event_semaphore);
    }
}

static void worker_task(void *parameters)
{
    (void) parameters;

    for (;;) {
        if (xSemaphoreTake(
                event_semaphore,
                portMAX_DELAY) == pdTRUE) {

            safe_print("Worker: event received\n");
        }
    }
}

BaseType_t demo_start(void)
{
    event_semaphore = xSemaphoreCreateBinary();
    console_mutex = xSemaphoreCreateMutex();

    if ((event_semaphore == NULL) ||
        (console_mutex == NULL)) {
        return pdFAIL;
    }

    if (xTaskCreate(
            producer_task,
            "Producer",
            configMINIMAL_STACK_SIZE,
            NULL,
            1,
            NULL) != pdPASS) {
        return pdFAIL;
    }

    if (xTaskCreate(
            worker_task,
            "Worker",
            configMINIMAL_STACK_SIZE,
            NULL,
            2,
            NULL) != pdPASS) {
        return pdFAIL;
    }

    return pdPASS;
}