# Light Aircraft Fuel Timer

This is a small (completely overkill) embedded project for monitoring fuel while
flying small aircraft like the Cessna 172, Piper Warrior, etc. It's currently
written against the Atmel AVR ATMega328P microcontroller, though the plan is to
eventually use the ATXMega128D4.

More project info can be found on [the project page on Hackaday.io](https://hackaday.io/project/19903-light-aircraft-fuel-timer).

## Developing / building

This project is set up for develop on Linux and OSX. It can probably be made to
work under AVR Studio on Windows with some work. It currently expects the target
to run at 16MHz, but `CLOCK` in the Makefile can be changed to whatever suits
your fuses.

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

# Flash with an AVR Dragon programmer
make flash PROGRAMMER="dragon_isp"
```

## Schematics / hardware

Details coming in future on how to actually. Currently this is sitting on a breadboard on my desk.