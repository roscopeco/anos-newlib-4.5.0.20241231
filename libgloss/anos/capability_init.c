/*
 * libanos capability initialisation for user mode
 * anos - An Operating System
 *
 * Copyright (c) 2025 Ross Bamford
 */

#include <stdint.h>
#include <string.h>

#define SYSCALL_CAP_COUNT ((128))

typedef struct {
    uint64_t syscall_id;
    uint64_t cookie;
} SyscallCapability;

// max 128 syscalls (we don't need that many...)
uint64_t __syscall_capabilities[SYSCALL_CAP_COUNT];

// this _could_ be a GCC constructor, but the order we
// execute them isn't necessarily guaranteed (we have priority,
// but arbitrary user code could also use that) so passing
// arguments would be a hack.
//
// So instead it's called directly by crt0, **before** constructors
// are run (to save faffing with the stack / saving & restoring 
// rdi and rsp registers).
//
void _anos_init_capabilities(int capc, SyscallCapability *capv) {    
    memset(__syscall_capabilities, 0, SYSCALL_CAP_COUNT);

    for (int i = 0; i < capc; i++) {
        __syscall_capabilities[capv[i].syscall_id] = capv[i].cookie;
    }
}