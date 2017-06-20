#!/usr/bin/env python

def FlagsForFile(filename, **kwargs):
    flags = [
        '-Wall',
        '-Wextra',
        '-Werror',
        '-Wno-attributes',
        '-std=c11',
        '-I.',
        '-isystem',
        '/usr/include',
        '-isystem',
        '/usr/local/include',
        '-isystem/usr/lib/avr/include',
        '-mmcu=atmega328p'
        '-DF_CPU=16000000L',
        '-D__AVR__',
        '-D__AVR_ATmega328P__',
    ]

    return {
        'flags': flags,
        'do_cache': True
    }
