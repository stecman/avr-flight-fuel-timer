DEVICE = atmega328p
CLOCK  = 8000000
PROGRAMMER = -c dragon_isp -B 1.5MHz

AVRDUDE = avrdude $(PROGRAMMER) -p $(DEVICE) -V

BUILD_DIR ?= build
BUILD_DIR_COMPILE = $(BUILD_DIR)/obj
BUILD_DIR_OUTPUT = $(BUILD_DIR)/output

# Main sources, excluding tests
SOURCES = $(shell find . -type f -name '*.c' \
						 -and -not -path './text.c' \
                         -and -not -path './tests/*')

# Ensure generated file is in sources (it might not exist yet)
SOURCES += text.c

OBJECTS := $(SOURCES:%=$(BUILD_DIR_COMPILE)/%.o)
DEPS := $(OBJECTS:.o=.d)

# Compiler flags
CFLAGS = -Wall -Os -DF_CPU=$(CLOCK) -mmcu=$(DEVICE)
CFLAGS += -I -I. -I$(shell pwd) -I./lib/
CFLAGS += -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -ffunction-sections -fdata-sections -Wl,--gc-sections
CFLAGS += -Wl,--relax -mcall-prologues
CFLAGS += -fno-tree-scev-cprop
CFLAGS += -fno-split-wide-types
CFLAGS += -std=gnu11 -Wstrict-prototypes

# Specfic warnings as errors
CFLAGS += -Werror=return-type

# Enable coloured output from avr-gcc
CFLAGS += -fdiagnostics-color=always

# Expose strings in source
CFLAGS += -DVERSION_STR="\"$(shell git describe --always)\""
CFLAGS += -DBUILD_DATE_STR="\"$(shell date '+%Y-%m-%d %H:%M')\""

# Track dependencies when building
CFLAGS += -MMD -MP

COMPILE = avr-gcc $(DEPFLAGS) $(CFLAGS)

TARGET = $(BUILD_DIR_OUTPUT)/fueltimer

# symbolic targets:
all: $(SOURCES) $(TARGET).hex

# Build a file of progmem strings
text.c: text.yml scripts/build_text.py
	./scripts/build_text.py text.yml

$(BUILD_DIR_COMPILE)/%.c.o: %.c
	@echo "Compiling $<"
	$(MKDIR_P) $(dir $@)
	@$(COMPILE) -c $< -o $@

$(BUILD_DIR_COMPILE)/%.S.o: %.S
	$(MKDIR_P) $(dir $@)
	$(COMPILE) -x assembler-with-cpp -c $< -o $@
	# "-x assembler-with-cpp" should not be necessary since this is the default
	# file type for the .S (with capital S) extension. However, upper case
	# characters are not always preserved on Windows. To ensure WinAVR
	# compatibility define the file type manually.

$(BUILD_DIR_COMPILE)/%.c.s: %.s
	$(MKDIR_P) $(dir $@)
	$(COMPILE) -S $< -o $@

$(TARGET).elf: $(OBJECTS)
	@echo "Linking $@"
	$(MKDIR_P) $(dir $@)
	@$(COMPILE) -o $(TARGET).elf $(OBJECTS)

$(TARGET).hex: $(TARGET).elf
	$(MKDIR_P) $(dir $@)
	@rm -f $(TARGET).hex
	avr-objcopy -j .text -j .data -O ihex $(TARGET).elf $(TARGET).hex
	avr-size --format=avr --mcu=$(DEVICE) $(TARGET).elf

test:
	$(MAKE) --no-print-directory -C tests test

flash: all
	$(AVRDUDE) -U flash:w:$(TARGET).hex:i

# Xcode uses the Makefile targets "", "clean" and "install"
install: flash

clean:
	# Remove build files
	rm -rf "$(BUILD_DIR)"

	# Remove generated files
	rm -f text.c text.h

	# Clean up test build
	$(MAKE) --no-print-directory -C tests clean

disasm:	main.elf
	avr-objdump -d $(TARGET).elf

# Empty target to force build on some targets
.PHONY: FORCE
.FORCE:

-include $(DEPS)

MKDIR_P ?= @mkdir -p