#include "formatters.h"
#include "text.h"

// AVR
#include <avr/pgmspace.h>
#include <string.h>
#include <stdlib.h>

void text_format_bool(uint8_t value, char* buffer, uint8_t length)
{
    if (value == 0) {
        strcpy_P(buffer, (const __flash char*) pstr_generic_no);
    } else {
        strcpy_P(buffer, (const __flash char*) pstr_generic_yes);
    }
}

void text_format_litres_per_hour(uint8_t value, char* buffer, uint8_t length)
{
	itoa(value, buffer, 10);
	strcat_P(buffer, (const __flash char*) ptr_suffix_litres_per_hour);
}

void text_format_minutes(uint8_t value, char* buffer, uint8_t length)
{
	itoa(value, buffer, 10);
	strcat_P(buffer, (const __flash char*) ptr_suffix_minutes);
}