/*
 * Anos system types
 * anos - An Operating System
 *
 * Copyright (c) 2024 Ross Bamford
 */

// clang-format Language: C

#ifndef __ANOS_ANOS_TYPES_H
#define __ANOS_ANOS_TYPES_H

#include <stddef.h>
#include <stdint.h>

typedef struct {
    uint64_t physical_total;
    uint64_t physical_avail;
} AnosMemInfo;

typedef void (*ThreadFunc)(void);

typedef struct {
    uintptr_t start;
    uint64_t len_bytes;
} ProcessMemoryRegion;

typedef struct {
    ThreadFunc entry_point;
    uintptr_t stack_base;
    size_t stack_size;
    uint8_t region_count;
    ProcessMemoryRegion *regions;
    uint16_t stack_value_count;
    uint64_t *stack_values;
    uint64_t reserved;
} ProcessCreateParams;

#endif //__ANOS_ANOS_TYPES_H
