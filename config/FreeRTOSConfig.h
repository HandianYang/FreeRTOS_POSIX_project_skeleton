/*
 * Minimal FreeRTOSConfig.h for the Posix (Linux) simulator port.
 *
 * Only the options actually needed by this project are set here; everything
 * else falls back to the kernel defaults in FreeRTOS.h. See
 * https://www.FreeRTOS.org/a00110.html for the full list of options.
 */
#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#include <pthread.h>    /* PTHREAD_STACK_MIN, used by configMINIMAL_STACK_SIZE. */

/* projCOVERAGE_TEST and projENABLE_TRACING are defined on the command line (see
 * the Makefile) so this header can be shared across build variants. */
#ifndef projCOVERAGE_TEST
    #error projCOVERAGE_TEST should be defined to 1 or 0 on the command line.
#endif

#ifndef projENABLE_TRACING
    #error projENABLE_TRACING should be defined to 1 or 0 on the command line.
#endif

/* Scheduler behaviour. */
#define configUSE_PREEMPTION                       1
#define configUSE_PORT_OPTIMISED_TASK_SELECTION    0
#define configTICK_RATE_HZ                         ( 1000 )
#define configMAX_PRIORITIES                       ( 7 )
#define configMAX_TASK_NAME_LEN                    ( 12 )
#define configUSE_16_BIT_TICKS                     0
#define configIDLE_SHOULD_YIELD                    1

/* Stack and heap sizing.
 *
 * configMINIMAL_STACK_SIZE is expressed in StackType_t words.  The Posix port
 * converts it to bytes (words * sizeof(StackType_t)) before handing it to
 * pthread_create(), which requires at least PTHREAD_STACK_MIN bytes.  Dividing
 * by sizeof(StackType_t) here makes a "minimal" task use exactly
 * PTHREAD_STACK_MIN bytes instead of 8x that. */
#define configMINIMAL_STACK_SIZE                   ( ( unsigned long ) ( PTHREAD_STACK_MIN / sizeof( StackType_t ) ) )
#define configTOTAL_HEAP_SIZE                      ( ( size_t ) ( 128 * 1024 ) )

/* Synchronisation primitives. configUSE_MUTEXES is required by
 * xSemaphoreCreateMutex(); binary semaphores need no extra option. */
#define configUSE_MUTEXES                          1

/* Memory allocation: dynamic only - the demo creates everything dynamically. */
#define configSUPPORT_DYNAMIC_ALLOCATION           1
#define configSUPPORT_STATIC_ALLOCATION            0

/* Optional features / hooks - all off for a minimal project. */
#define configUSE_IDLE_HOOK                        0
#define configUSE_TICK_HOOK                        0
#define configUSE_TIMERS                           0
#define configUSE_CO_ROUTINES                      0
#define configCHECK_FOR_STACK_OVERFLOW             0
#define configUSE_MALLOC_FAILED_HOOK               0

/* API functions to include (everything else defaults to excluded). */
#define INCLUDE_vTaskDelay                         1

/* configASSERT() is cheap insurance while developing.  projCOVERAGE_TEST builds
 * deliberately leave it out so that "can never happen" branches still execute. */
#if ( projCOVERAGE_TEST != 1 )
    extern void vAssertCalled( const char * const pcFileName,
                               unsigned long ulLine );
    #define configASSERT( x )    if( ( x ) == 0 ) vAssertCalled( __FILE__, __LINE__ )
#endif

#endif /* FREERTOS_CONFIG_H */
