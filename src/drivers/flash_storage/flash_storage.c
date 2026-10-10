#include "flash_storage.h"

#include <string.h>

#include "hardware/flash.h"
#include "hardware/regs/addressmap.h"
#include "pico/error.h"
#include "pico/flash.h"
#include "pico/platform.h"

#ifndef PICO_FLASH_SIZE_BYTES
#error "PICO_FLASH_SIZE_BYTES must be defined for the selected board"
#endif

#define FLASH_STORAGE_SAFE_EXECUTE_TIMEOUT_MS 1000U
extern uint8_t __flash_binary_end;

typedef struct {
    uint32_t flash_offset;
    size_t length;
    const uint8_t *source;
} flash_storage_program_request_t;

typedef struct {
    uint32_t flash_offset;
    size_t length;
} flash_storage_erase_request_t;

static bool range_is_valid(const flash_storage_t *storage, size_t offset, size_t length)
{
    return (storage != NULL) && storage->initialized && (length > 0U) &&
           (offset <= storage->capacity) && (length <= (storage->capacity - offset));
}

static bool buffer_is_in_sram(const void *buffer, size_t length)
{
    const uintptr_t buffer_start = (uintptr_t)buffer;
    const uintptr_t sram_start = (uintptr_t)SRAM_BASE;
    const uintptr_t sram_end = (uintptr_t)SRAM_END;

    return (length > 0U) && (buffer_start >= sram_start) &&
           (buffer_start < sram_end) &&
           (length <= (size_t)(sram_end - buffer_start));
}

static void __not_in_flash_func(erase_flash_region)(void *context)
{
    const flash_storage_erase_request_t *request = (const flash_storage_erase_request_t *)context;
    flash_range_erase(request->flash_offset, request->length);
}

static void __not_in_flash_func(program_flash_region)(void *context)
{
    const flash_storage_program_request_t *request = (const flash_storage_program_request_t *)context;
    flash_range_program(request->flash_offset, request->source, request->length);
}

bool flash_storage_init(flash_storage_t *storage, const flash_storage_region_t *region)
{
    if (storage == NULL) {
        return false;
    }

    storage->initialized = false;
    storage->flash_offset = 0U;
    storage->capacity = 0U;

    if ((region == NULL) || (region->capacity == 0U) ||
        ((region->flash_offset % FLASH_STORAGE_SECTOR_SIZE) != 0U) ||
        ((region->capacity % FLASH_STORAGE_SECTOR_SIZE) != 0U) ||
        (region->flash_offset > PICO_FLASH_SIZE_BYTES) ||
        (region->capacity > ((size_t)PICO_FLASH_SIZE_BYTES - region->flash_offset))) {
        return false;
    }

    const uintptr_t binary_end_address = (uintptr_t)&__flash_binary_end;
    if (binary_end_address < (uintptr_t)XIP_BASE) {
        return false;
    }

    const uintptr_t binary_end_offset = binary_end_address - (uintptr_t)XIP_BASE;
    if (binary_end_offset > (uintptr_t)PICO_FLASH_SIZE_BYTES) {
        return false;
    }

    const uintptr_t first_available_sector =
        (binary_end_offset + FLASH_STORAGE_SECTOR_SIZE - 1U) / FLASH_STORAGE_SECTOR_SIZE;
    if ((uintptr_t)region->flash_offset < (first_available_sector * FLASH_STORAGE_SECTOR_SIZE)) {
        return false;
    }

    storage->flash_offset = region->flash_offset;
    storage->capacity = region->capacity;
    storage->initialized = true;
    return true;
}

bool flash_storage_read(const flash_storage_t *storage, size_t offset, void *dst, size_t length)
{
    if (!range_is_valid(storage, offset, length) || (dst == NULL) ||
        !buffer_is_in_sram(dst, length)) {
        return false;
    }

    const uint8_t *flash_address = (const uint8_t *)(XIP_BASE + storage->flash_offset + offset);
    (void)memcpy(dst, flash_address, length);
    return true;
}

bool flash_storage_erase(const flash_storage_t *storage, size_t offset, size_t length)
{
    if (!range_is_valid(storage, offset, length) ||
        ((offset % FLASH_STORAGE_SECTOR_SIZE) != 0U) ||
        ((length % FLASH_STORAGE_SECTOR_SIZE) != 0U)) {
        return false;
    }

    const flash_storage_erase_request_t request = {
        .flash_offset = storage->flash_offset + (uint32_t)offset,
        .length = length,
    };
    return flash_safe_execute(erase_flash_region, (void *)&request,
                              FLASH_STORAGE_SAFE_EXECUTE_TIMEOUT_MS) == PICO_OK;
}

bool flash_storage_write(const flash_storage_t *storage, size_t offset,
                         const uint8_t *src, size_t length)
{
    if (!range_is_valid(storage, offset, length) || (src == NULL) ||
        ((offset % FLASH_STORAGE_PAGE_SIZE) != 0U) ||
        ((length % FLASH_STORAGE_PAGE_SIZE) != 0U)) {
        return false;
    }

    if (!buffer_is_in_sram(src, length)) {
        return false;
    }

    const flash_storage_program_request_t request = {
        .flash_offset = storage->flash_offset + (uint32_t)offset,
        .length = length,
        .source = src,
    };
    return flash_safe_execute(program_flash_region, (void *)&request,
                              FLASH_STORAGE_SAFE_EXECUTE_TIMEOUT_MS) == PICO_OK;
}

size_t flash_storage_capacity(const flash_storage_t *storage)
{
    return ((storage != NULL) && storage->initialized) ? storage->capacity : 0U;
}
