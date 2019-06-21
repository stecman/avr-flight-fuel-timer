#pragma once

#include <stdint.h>
#include <stdbool.h>

enum FramOpStatus {
    kFram_Success = 0x0,
    kFram_BadChecksum,
    kFram_Failed,
} typedef FramOpStatus;

/**
 * Configure the F-RAM initial state
 */
void fram_init(void);

/**
 * Write an arbitrary data structure to F-RAM
 * Note that the stored data will occupy (size + 2) bytes as a checksum is used
 */
FramOpStatus fram_write(uint16_t address, void* data, uint8_t size);

/**
 * Read into an arbitrary data structure from the current address
 */
FramOpStatus fram_read(uint16_t address, uint8_t size, void* output);