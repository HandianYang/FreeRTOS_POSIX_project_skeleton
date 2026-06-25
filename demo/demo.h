#ifndef DEMO_H
#define DEMO_H

#include "FreeRTOS.h"

/* Uniform entry point implemented by every demo module. The generic runner
 * (demo/runner.c) calls it before starting the scheduler. Exactly one demo is
 * linked into a demo binary, so the symbol never clashes. */
BaseType_t demo_start(void);

#endif /* DEMO_H */
