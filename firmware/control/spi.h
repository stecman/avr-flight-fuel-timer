#pragma once

#include <stdint.h>

/**
 * Configure the SPI peripheral initial state
 */
void spi_init(void);

void spi_write(const uint8_t data);

uint8_t spi_read(void);