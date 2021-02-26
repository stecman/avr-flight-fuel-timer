TARGET_EXEC ?= testsuite

# Tests are built in an additional subdir so firmware sources ("../") end up in build/
BUILD_DIR ?= build
BUILD_DIR_INTERNAL ?= $(BUILD_DIR)/tests

TARGET = $(BUILD_DIR)/$(TARGET_EXEC)

# Test sources
SRCS := suite.c munit/munit.c $(shell find -iname 'test_*.c')

# Firmware sources
SRCS += ../timer/math.c

OBJS := $(SRCS:%=$(BUILD_DIR_INTERNAL)/%.o)
DEPS := $(OBJS:.o=.d)

CFLAGS += -Wall -std=gnu99
CFLAGS = -I -I. -I$(shell realpath `pwd`/../)

# Ignore __flash attribute when compiling for tests
CFLAGS += -D__flash=""

# Track dependencies
CFLAGS += -MMD -MP

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR_INTERNAL)/%.c.o: %.c
	@echo "Compiling $<"
	$(MKDIR_P) $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean test

test: $(TARGET)
	@./$(TARGET)

clean:
	$(RM) -r $(BUILD_DIR_INTERNAL)

-include $(DEPS)

MKDIR_P ?= @mkdir -p

