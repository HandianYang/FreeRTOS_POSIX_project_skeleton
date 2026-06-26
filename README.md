<!-- This English README is the source of truth. When you change it, update README.zh-TW.md too. -->

**English** | [繁體中文](README.zh-TW.md)

# FreeRTOS Linux/Posix Project Skeleton

A minimal project skeleton built on FreeRTOS's Posix (Linux) simulator port.
The production application and each demo are compiled into **separate, independent
executables** that never interfere with one another.

## Layout

```
config/
  FreeRTOSConfig.h    Trimmed kernel config (only the options this project needs)
FreeRTOS/             FreeRTOS kernel sources and the Posix port (do not modify)
project/              Production application
  main.c              Production entry point: calls app_start(), then starts the scheduler
  app.c / app.h       Production logic app_start() (create your tasks here)
  hook.c              FreeRTOS hooks (vAssertCalled…, shared by both builds)
demo/                 Self-contained demos
  runner.c            Generic demo entry point: calls demo_start(), then starts the scheduler
  demo.h              Uniform interface: BaseType_t demo_start(void);
  blinky_demo.c       Example demo copied from FreeRTOS; `task`, `timers`, and `semphr` are required and used in this demo.
  semaphore_demo.c    Example demo implementing binary semaphores; `task` and `semphr` are required and used in this demo.
  
Makefile
README.md             English documentation
README.zh-TW.md       Traditional Chinese documentation
```

### Design notes

- An executable can have only one `main()` and one call to `vTaskStartScheduler()`,
  so **production and demos are linked into mutually exclusive binaries**.
- `main()` only handles the boot sequence; the real logic lives in `app_start()`
  (production) or `demo_start()` (demo).
- `hook.c` provides the application-level callbacks the kernel requires (currently
  `vAssertCalled`, invoked when a `configASSERT` fails); it is linked into both builds.
- The kernel and hook `.o` files are shared, so they are compiled only once.

## Build and run

| Command | Output | Description |
|---|---|---|
| `make` or `make app` | `build/app` | Build the production application |
| `make run` | — | Build and run the production application |
| `make demo` | `build/blinky_demo` | Build the default demo (BLINKY_DEMO) |
| `make run-demo` | — | Build and run the default demo |
| `make demo DEMO=semaphore` | `build/semaphore_demo` | Build `demo/semaphore_demo.c` |
| `make run-demo DEMO=semaphore` | — | Build and run the given demo |
| `make clean` | — | Remove `build/` |

The `DEMO` variable selects which demo module to build; it defaults to `blinky`.

> Note: a demo runs an infinite loop — stop it with `Ctrl-C`. If you observe output
> with `timeout 3 ./build/blinky_demo`, exit code `124` means `timeout` stopped it
> normally and is not an error.

## How to add or modify code

### A. Develop the production application

1. In [project/app.c](project/app.c), create your tasks, queues, semaphores, etc.
   inside `app_start()`. Return `pdPASS` on success, `pdFAIL` on failure.
2. When splitting code into more files, put the new `.c` under `project/` and add it
   to `APP_SOURCES` in the Makefile.
3. Run `make run` to build and run. `main.c` usually needs no changes.

### B. Add a new demo

1. Add `demo/<name>_demo.c` and implement:

   ```c
   #include "demo.h"
   /* ... any headers you need, e.g. task.h, semphr.h ... */

   BaseType_t demo_start(void)
   {
       /* Create this demo's tasks, etc. Return pdPASS on success. */
       return pdPASS;
   }
   ```

2. **No Makefile change is needed** — just run:

   ```bash
   make run-demo DEMO=<name>
   ```

   `DEMO=<name>` maps to `demo/<name>_demo.c`.

> Convention: every demo's public function is named `demo_start`. Since exactly one
> demo is linked at a time, the name never clashes; keep a demo's other functions
> `static`.

### C. Modify an existing demo

Edit the relevant `demo/<name>_demo.c`, then run `make run-demo DEMO=<name>`.

## Configuration (FreeRTOSConfig.h)

[config/FreeRTOSConfig.h](config/FreeRTOSConfig.h) enables only the options this
project uses and leaves the rest at the kernel defaults. When enabling a new feature,
remember to add both the matching option and its source file.

> `projCOVERAGE_TEST` and `projENABLE_TRACING` are defined on the command line by the
> Makefile via `-D`; do not hard-code them in the config file.