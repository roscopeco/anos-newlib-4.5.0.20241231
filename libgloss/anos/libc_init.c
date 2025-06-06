/*
 * libc initialisation for user mode
 * anos - An Operating System
 *
 * Copyright (c) 2025 Ross Bamford
 * 
 * TODO this is RISC-V-specific right now, 
 * either generalise it or move it!
 */

#include <stddef.h>

extern void (*__preinit_array_start []) (void) __attribute__((weak));
extern void (*__preinit_array_end []) (void) __attribute__((weak));
extern void (*__init_array_start []) (void) __attribute__((weak));
extern void (*__init_array_end []) (void) __attribute__((weak));
extern void (*__fini_array_start []) (void) __attribute__((weak));
extern void (*__fini_array_end []) (void) __attribute__((weak));

void _anos_libc_init_array (void) {
    size_t count;
    size_t i;

    count = __preinit_array_end - __preinit_array_start;
    for (i = 0; i < count; i++) {
        __preinit_array_start[i] ();
    }

#ifdef HAVE_INIT_FINI
    _init ();
#endif

    count = __init_array_end - __init_array_start;
    for (i = 0; i < count; i++) {
        __init_array_start[i] ();
    }
}

void _anos_libc_fini_array (void) {
    size_t count;
    size_t i;

    count = __fini_array_end - __fini_array_start;
    for (i = 0; i < count; i++) {
        __fini_array_start[i] ();
    }

#ifdef HAVE_INIT_FINI
    _fini ();
#endif
}