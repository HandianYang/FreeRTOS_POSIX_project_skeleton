<!-- Translation of README.md. The English version is the source of truth and may be more up to date. -->

[English](README.md) | **繁體中文**

# FreeRTOS Linux/Posix 專案骨架

以 FreeRTOS 的 Posix（Linux）模擬器 port 為基礎的最小專案骨架。
正式專案程式與各種 demo 會編成**各自獨立的執行檔**，彼此不互相干擾。

## 檔案架構

```
config/
  FreeRTOSConfig.h    精簡過的核心設定（只開本專案需要的選項）
FreeRTOS/             FreeRTOS kernel 原始碼與 Posix port（不要修改，除非欲加入更多 kernel 功能）
project/              正式專案
  main.c              production 進入點：呼叫 app_start() 後啟動排程
  app.c / app.h       正式應用邏輯 app_start()（在這裡建立你的 tasks）
  hook.c              FreeRTOS hooks（vAssertCalled…,兩種建置共用）
demo/                 各種獨立 demo
  runner.c            demo 通用進入點：呼叫 demo_start() 後啟動排程
  demo.h              統一介面：BaseType_t demo_start(void);
  blinky_demo.c       從 FreeRTOS 複製過來的範例；此 demo 需要 `task`、`timers`、`semphr`。
  semaphore_demo.c    實作 binary semaphore 的範例；此 demo 需要 `task`、`semphr`。
Makefile
README.md             英文文件
README.zh-TW.md       繁體中文文件
```

### 設計重點

- 一個執行檔只能有一個 `main()` 與一次 `vTaskStartScheduler()`，因此
  **production 與 demo 是互斥地連結成不同 binary**。
- `main()` 只負責開機流程；實際邏輯放在 `app_start()`（正式專案）或 `demo_start()`（demo）。
- `hook.c` 提供應用層必須實作的回呼（目前是 `configASSERT` 失敗時呼叫的 `vAssertCalled`），兩種建置都會連結。
- kernel 與 hook 的 `.o` 兩邊共用,只會編譯一次。

## 建置與執行

| 指令 | 產物 | 說明 |
|---|---|---|
| `make` 或 `make app` | `build/app` | 建置正式專案 |
| `make run` | — | 建置並執行正式專案 |
| `make demo` | `build/blinky_demo` | 建置預設 demo(blinky) |
| `make run-demo` | — | 建置並執行預設 demo |
| `make demo DEMO=semaphore` | `build/semaphore_demo` | 建置 `demo/semaphore_demo.c` |
| `make run-demo DEMO=semaphore` | — | 建置並執行指定 demo |
| `make clean` | — | 刪除 `build/` |

`DEMO` 變數選擇要建哪一個 demo 模組,預設為 `blinky`。

> 註：demo 是無窮迴圈程式,用 `Ctrl-C` 結束；若用 `timeout 3 ./build/blinky_demo` 觀察輸出，離開碼 `124` 代表是 `timeout` 正常中止，並非錯誤。

## 新增或修改程式的步驟

### A. 開發正式專案

1. 在 [project/app.c](project/app.c) 的 `app_start()` 裡建立你的 tasks, queues, semaphores 等，成功回傳 `pdPASS`、失敗回傳 `pdFAIL`。
2. 程式拆分時，把新的 `.c` 放在 `project/` 下，並加進 Makefile 的
   `APP_SOURCES`。
3. `make run` 建置並執行。`main.c` 通常不需要再改動。

### B. 新增一個 demo

1. 新增 `demo/<名稱>_demo.c`，在裡面實作：

   ```c
   #include "demo.h"
   /* ... 其他需要的 header，例如 task.h, semphr.h ... */

   BaseType_t demo_start(void)
   {
       /* 建立此 demo 的 tasks 等，成功回傳 pdPASS */
       return pdPASS;
   }
   ```

2. **不用改 Makefile**，直接執行：

   ```bash
   make run-demo DEMO=<名稱>
   ```

   `DEMO=<名稱>` 會對應到 `demo/<名稱>_demo.c`。

> 慣例：每個 demo 的對外函式一律命名為 `demo_start`。因為一次只連結一個 demo，所以不會撞名；demo 內部的其他函式請用 `static`。

### C. 修改既有 demo

直接編輯該 `demo/<名稱>_demo.c`，再 `make run-demo DEMO=<名稱>` 即可。

## 設定檔(FreeRTOSConfig.h)

[config/FreeRTOSConfig.h](config/FreeRTOSConfig.h) 只開啟本專案用到的選項,其餘交給
kernel 預設值。若要啟用新功能,記得同時補上對應設定與來源檔。

> `projCOVERAGE_TEST` 與 `projENABLE_TRACING` 由 Makefile 以 `-D` 在命令列定義，不要寫死在設定檔裡。
