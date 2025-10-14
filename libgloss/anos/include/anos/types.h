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

typedef void (*ProcessEntrypointFunc)(void);

typedef struct {
    uintptr_t start;
    uint64_t len_bytes;
} ProcessMemoryRegion;

typedef enum {
    TASK_CLASS_IDLE = 0,
    TASK_CLASS_NORMAL,
    TASK_CLASS_HIGH,
    TASK_CLASS_REALTIME,

	// Keep this last
	TASK_CLASS_INVALID,
} __attribute__((packed)) TaskClass;

typedef struct {
    ProcessEntrypointFunc entry_point;	// 8
    uintptr_t stack_base;               // 16
    size_t stack_size;                  // 24
    uint8_t region_count;               // 25
    TaskClass task_class;               // 26
    uint8_t reserved0[6];               // 32
    ProcessMemoryRegion *regions;       // 40
    uint16_t stack_value_count;         // 42
    uint16_t reserved1[3];              // 48
    uint64_t *stack_values;             // 56
    uint64_t reserved;                  // 64
} __attribute__((packed)) ProcessCreateParams;

static_assert(sizeof(ProcessCreateParams) == 64);

#endif //__ANOS_ANOS_TYPES_H
