/*
 * Include for Anos user-mode supervisors
 * anos - An Operating System
 *
 * Copyright (c) 2024 Ross Bamford
 */

// clang-format Language: C

#ifndef __ANOS_ANOS_SYSTEM_H
#define __ANOS_ANOS_SYSTEM_H

#include <stdint.h>

typedef struct {
    uint64_t some_number;
} AnosSystemRegistration;

const char *libanos_version();

void anos_task_sleep_current_secs(uint64_t secs);

#endif //__ANOS_ANOS_SYSTEM_H
