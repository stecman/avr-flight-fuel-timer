DEVICE = atmega328p
CLOCK  = 16000000
PROGRAMMER = -c arduino -P /dev/ttyUSB0 -b57600

AVRDUDE = avrdude $(PROGRAMMER) -p $(DEVICE)

# Main sources, excluding tests
SOURCES = $(shell find . -type f -name '*.c' \
						 -and -not -path './text.c' \
                         -and -not -path './tests/*')

# Ensure generated file is in sources (it might not exist yet)
SOURCES += text.c

OBJECTS = $(SOURCES:.c=.o)

# Automatic dependency resolution
DEPDIR := .d
$(shell mkdir -p $(DEPDIR) >/dev/null)
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.Td

# Compiler flags
CFLAGS = -Wall -Os -DF_CPU=$(CLOCK) -mmcu=$(DEVICE)
CFLAGS += -I -I. -I$(shell pwd) -I./lib/
CFLAGS += -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -ffunction-sections -fdata-sections -Wl,--gc-sections
CFLAGS += -Wl,--relax -mcall-prologues
CFLAGS += -std=gnu11 -Wstrict-prototypes

# Specfic warnings as errors
CFLAGS += -Werror=return-type

# Enable coloured output from avr-gcc
CFLAGS += -fdiagnostics-color=always

# Expose strings in source
CFLAGS += -DVERSION_STR="\"$(shell git describe --always)\""
CFLAGS += -DBUILD_DATE_STR="\"$(shell date '+%Y-%m-%d %H:%M')\""

COMPILE = avr-gcc $(DEPFLAGS) $(CFLAGS)
POSTCOMPILE = mv -f $(DEPDIR)/$*.Td $(DEPDIR)/$*.d && touch $@

# symbolic targets:
all: $(SOURCES) main.hex

# Build a file of progmem strings
text.c: text.yml scripts/build_text.py
	./scripts/build_text.py text.yml

.c.o: $(DEPDIR)/%.d
	@echo "Compiling $<"
	@mkdir -p "$(DEPDIR)/$(shell dirname $@)"
	@$(COMPILE) -c $< -o $@
	@$(POSTCOMPILE)

.S.o:
	$(COMPILE) -x assembler-with-cpp -c $< -o $@
	# "-x assembler-with-cpp" should not be necessary since this is the default
	# file type for the .S (with capital S) extension. However, upper case
	# characters are not always preserved on Windows. To ensure WinAVR
	# compatibility define the file type manually.

.c.s:
	$(COMPILE) -S $< -o $@

test:
	$(MAKE) --no-print-directory -C tests

flash: all
	$(AVRDUDE) -U flash:w:main.hex:i

# Xcode uses the Makefile targets "", "clean" and "install"
install: flash

clean:
	# Remove intermediates
	find . -name '*.d' -or -name '*.o' -exec rm {} +

	# Remove generated files
	rm -f text.c text.h

	# Remove binaries
	rm -f main.hex main.elf

	# Remove dependency tracking files
	rm -rf "$(DEPDIR)"

	# Clean up test build
	$(MAKE) --no-print-directory -C tests clean

main.elf: $(OBJECTS)
	@echo "Linking $@"
	@$(COMPILE) -o main.elf $(OBJECTS)

main.hex: main.elf
	@rm -f main.hex
	avr-objcopy -j .text -j .data -O ihex main.elf main.hex
	avr-size --format=avr --mcu=$(DEVICE) main.elf

disasm:	main.elf
	avr-objdump -d main.elf

# Empty target to force build on some targets
.PHONY: FORCE
.FORCE:

$(DEPDIR)/%.d: ;
.PRECIOUS: $(DEPDIR)/%.d

include $(wildcard $(patsubst %,$(DEPDIR)/%.d,$(basename $(SOURCES))))
