DEVICE = atmega328p
CLOCK  = 16000000
PROGRAMMER = -c arduino -P /dev/ttyUSB0 -b57600

AVRDUDE = avrdude $(PROGRAMMER) -p $(DEVICE)

SOURCES = $(shell find . -name '*.c' -and -not -name text.c) text.c
OBJECTS = $(SOURCES:.c=.o)

COMPILE = avr-gcc -Wall -Os -DF_CPU=$(CLOCK) -mmcu=$(DEVICE)
COMPILE += -I -I. -I$(shell pwd) -I./lib/
COMPILE += -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
COMPILE += -ffunction-sections -fdata-sections -Wl,--gc-sections
COMPILE += -Wl,--relax -mcall-prologues
COMPILE += -std=gnu99 -Wstrict-prototypes

# Enable coloured output from avr-gcc
COMPILE += -fdiagnostics-color=always

# Expose strings in source
COMPILE += -DVERSION_STR="\"$(shell git describe --always)\""
COMPILE += -DBUILD_DATE_STR="\"$(shell date '+%Y-%m-%d %H:%M')\""

# symbolic targets:
all: $(SOURCES) main.hex

text.c: text.yml
	# Build a file of progmem strings
	./scripts/build_text.py text.yml

.c.o:
	$(COMPILE) -c $< -o $@

.S.o:
	$(COMPILE) -x assembler-with-cpp -c $< -o $@
	# "-x assembler-with-cpp" should not be necessary since this is the default
	# file type for the .S (with capital S) extension. However, upper case
	# characters are not always preserved on Windows. To ensure WinAVR
	# compatibility define the file type manually.

.c.s:
	$(COMPILE) -S $< -o $@

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

main.elf: $(OBJECTS)
	$(COMPILE) -o main.elf $(OBJECTS)

main.hex: main.elf
	rm -f main.hex
	avr-objcopy -j .text -j .data -O ihex main.elf main.hex
	avr-size --format=avr --mcu=$(DEVICE) main.elf

disasm:	main.elf
	avr-objdump -d main.elf

cpp:
	$(COMPILE) -E main.c
