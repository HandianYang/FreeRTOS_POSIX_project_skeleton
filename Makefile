CC := gcc

PROJ_DIR   := project
DEMO_DIR   := demo
CONFIG_DIR := config
KERNEL_DIR := FreeRTOS
PORT_DIR   := $(KERNEL_DIR)/portable/Posix
HEAP_DIR   := $(KERNEL_DIR)/portable/MemMang
BUILD_DIR  := build

CFLAGS := \
	-std=gnu11 \
	-Wall \
	-Wextra \
	-Wpedantic \
	-g \
	-O0 \
	-pthread \
	-DprojCOVERAGE_TEST=0 \
	-DprojENABLE_TRACING=0

CPPFLAGS := \
	-I$(CONFIG_DIR) \
	-I$(KERNEL_DIR)/include \
	-I$(PORT_DIR) \
	-I$(PORT_DIR)/utils \
	-I$(PROJ_DIR) \
	-I$(DEMO_DIR)

LDFLAGS := -pthread

# Kernel + hooks: shared by every build, contains no main().
COMMON_SOURCES := \
	$(PROJ_DIR)/hook.c \
	$(KERNEL_DIR)/console.c \
	$(KERNEL_DIR)/list.c \
	$(KERNEL_DIR)/queue.c \
	$(KERNEL_DIR)/tasks.c \
	$(KERNEL_DIR)/timers.c \
	$(PORT_DIR)/port.c \
	$(PORT_DIR)/utils/wait_for_event.c \
	$(HEAP_DIR)/heap_4.c

# Production application: main() lives in main.c.
APP_SOURCES := \
	$(PROJ_DIR)/main.c \
	$(PROJ_DIR)/app.c

# Demo: generic runner + the selected demo module. Override with e.g.
#   make demo DEMO=queue
DEMO ?= blinky
DEMO_SOURCES := \
	$(DEMO_DIR)/runner.c \
	$(DEMO_DIR)/$(DEMO)_demo.c

APP_TARGET  := $(BUILD_DIR)/app
DEMO_TARGET := $(BUILD_DIR)/$(DEMO)_demo

APP_OBJECTS  := $(patsubst %.c,$(BUILD_DIR)/%.o,$(APP_SOURCES) $(COMMON_SOURCES))
DEMO_OBJECTS := $(patsubst %.c,$(BUILD_DIR)/%.o,$(DEMO_SOURCES) $(COMMON_SOURCES))
DEPENDS := $(APP_OBJECTS:.o=.d) $(DEMO_OBJECTS:.o=.d)

.PHONY: all app demo run run-demo clean

all: app

app: $(APP_TARGET)
demo: $(DEMO_TARGET)

$(APP_TARGET): $(APP_OBJECTS)
	@mkdir -p $(dir $@)
	$(CC) $(APP_OBJECTS) $(LDFLAGS) -o $@

$(DEMO_TARGET): $(DEMO_OBJECTS)
	@mkdir -p $(dir $@)
	$(CC) $(DEMO_OBJECTS) $(LDFLAGS) -o $@

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -MMD -MP -c $< -o $@

run: app
	./$(APP_TARGET)

run-demo: demo
	./$(DEMO_TARGET)

clean:
	rm -rf $(BUILD_DIR)

-include $(DEPENDS)
