#ifndef FLASH_STORAGE_H
#define FLASH_STORAGE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/** RP2040 flash erase sector size in bytes. */
#define FLASH_STORAGE_SECTOR_SIZE 4096U
/** RP2040 flash program page size in bytes. */
#define FLASH_STORAGE_PAGE_SIZE 256U

/**
 * Caller-owned descriptor for one reserved region in the onboard flash.
 *
 * Initialize this object with flash_storage_init() before using it. The
 * caller must ensure the region is reserved from all other firmware/data
 * uses, including any image or asset written by the update process.
 */
typedef struct {
    uint32_t flash_offset;
    size_t capacity;
    bool initialized;
} flash_storage_t;

/** Region supplied to flash_storage_init(), in bytes from the start of flash. */
typedef struct {
    uint32_t flash_offset;
    size_t capacity;
} flash_storage_region_t;

/**
 * Validate and bind a caller-reserved region to a storage descriptor.
 * The region start and capacity must be sector aligned, and its start must
 * follow the linked firmware image.
 */
bool flash_storage_init(flash_storage_t *storage, const flash_storage_region_t *region);

/** Read any non-empty byte range from the configured region. */
bool flash_storage_read(const flash_storage_t *storage, size_t offset, void *dst, size_t length);

/** Erase a non-empty sector-aligned range in the configured region. */
bool flash_storage_erase(const flash_storage_t *storage, size_t offset, size_t length);

/**
 * Program a non-empty page-aligned range in the configured region.
 * The source must be readable from SRAM while flash is unavailable; it must
 * not point into the XIP flash address space.
 */
bool flash_storage_write(const flash_storage_t *storage, size_t offset,
                         const uint8_t *src, size_t length);

/** Return the configured region capacity, or zero if storage is uninitialized. */
size_t flash_storage_capacity(const flash_storage_t *storage);

#endif /* FLASH_STORAGE_H */
