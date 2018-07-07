# Light Aircraft Fuel Timer

This is a small (completely overkill) embedded project for monitoring fuel while
flying small aircraft like the Cessna 172, Piper Warrior, etc. It's currently
written against the Atmel AVR ATMega328P microcontroller, though the plan is to
eventually use the ATXMega128D4.

I have some practical use for this, but it's mostly an experiment in how much UI
can be crammed onto a medium-sized 8-bit micro.

More project info can be found on [the project page on Hackaday.io](https://hackaday.io/project/19903-light-aircraft-fuel-timer).

## Building

This project is set up for develop on Linux and OSX. It can probably be made to
work under AVR Studio on Windows with some work. It currently needs to be built
using `-std=gnu99`. The target is expected to run at 16MHz, but `CLOCK` can be
configured in the Makefile to whatever suits your fuses.

On a Debian-based Linux distro:

```sh
# Install build and flashing tools
sudo apt-get install gcc-avr avrdude binutils-avr

# Install Python if you don't already have it for some reason
sudo apt-get install python python-pip

# Install the PyYAML module (required for some code generation)
pip install pyyaml

# Build the project
make
```

### Flashing a device

`avrdude` options for your programmer can be specified with `PROGRAMMER`:

```sh
# Flash a device connected via serial with an Arudino bootloader
make flash PROGRAMMER="-c arduino -P /dev/ttyUSB0 -b57600"

# Flash with an AVR Dragon programmer in ISP mode
make flash PROGRAMMER="dragon_isp"
```

## Notes for users

- Fuel and fuel burn per hour are represented as whole integers. Partly because
  this device is for approximation, and partly because the precision of a fuel
  tank dipstick or a "fuel burn per hour" average/max is unlikely to be greater
  than whole numbers.

  If you want better accuracy, get avionics that have a digital fuel flow meter
  and fuel level sensors that can work out your actual burn and endurance.

## Developer notes

- Strings are stored in `PROGMEM` (`__flash` under `gnu99`). These are defined in
  `text.yml` and turned  into C header and source files during build, as the C code
  to ensure reuse of strings in flash is verbose and repetitive.

- There is no dynamic allocation in this project. This design choice is partly
  to avoid hard to reproduce bugs that are possible with heap allocation on
  microcontrollers, and partly as a fun challenge.

- State exists statically in a few places (eg. `system.c`). Global variables
  are avoided in favour of private state with an exposed interface. Functionality
  is defined independently of this state where possible (eg. `eventloop.h`).

- An event loop is currently used to handle user input. This is an experiment
  and is a bit limited (no arguments, small event queue, etc). Not entirely sure
  this is appropriate for an embedded device, but it's nice to work with for now.

- This is my first pure C project. There may be oddities because of this -
  feedback is appreciated.

## Schematics / hardware

Details coming in future on how to put this circuit/device together. Currently
this is sitting on a breadboard on my desk.