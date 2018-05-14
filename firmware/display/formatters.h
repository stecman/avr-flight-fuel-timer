#include "u8glib/src/u8g.h"

#include <stdint.h>

/**
 * Return a human readable string for a boolean
 */
void text_format_bool(uint8_t value, char* buffer, uint8_t length);

/**
 * Return an integer as a string "123 L/h"
 */
void text_format_litres_per_hour(uint8_t value, char* buffer, uint8_t length);

/**
 * Return an integer as a string in minutes "44m"
 */
void text_format_minutes(uint8_t value, char* buffer, uint8_t length);