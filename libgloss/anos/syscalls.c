/*
 * Anos system call interface for user-mode code
 * anos - An Operating System
 *
 * Copyright (c) 2024 Ross Bamford
 */

#include <anos/syscalls.h>

#include <stdatomic.h>
#include <stddef.h>
#include <stdint.h>
#include <stdnoreturn.h>

#include <sys/types.h>
#include <sys/times.h>

#include <errno.h>
#undef errno
extern int errno;

#define PAGE_SIZE 0x1000
#define ALIGN_UP(x)   (((x) + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1))
#define ALIGN_DOWN(x) ((x) & ~(PAGE_SIZE - 1))

// These are your heap region limits (replace with actual values)
#define HEAP_START (((uintptr_t)&_bss_end + 0x4000))
#define HEAP_LIMIT (((uintptr_t)0x00000000bfffffff))

extern void *_bss_end;
static _Atomic uintptr_t __heap_end = (uintptr_t)&_bss_end + 0x40000;

noreturn void _exit(int) {
    while (1);

    __builtin_unreachable();
}

int _close(int file) {
    errno = EBADF;
    return -1;
}

int _execve(char *name, char **argv, char **env) {
    errno = ENOSYS;
    return -1;
}

int _fork() {
    errno = ENOSYS;
    return -1;
}

int _fstat(int file, struct stat *st) {
    errno = ENOSYS;
    return -1;
}

int _getpid() {
    return 1;
}

int _isatty(int file) {
    return 0;
}

int _kill(int pid, int sig) {
    errno = EINVAL;
    return -1;
}

int _link(char *old, char *new) {
    errno = EMLINK;
    return -1;
}

int _lseek(int file, int ptr, int dir) {
    errno = ENOSYS;
    return -1;
}

int _open(const char *name, int flags, ...) {
    errno = ENOENT;
    return -1;
}

int _read(int file, char *ptr, int len) {
    errno = ENOENT;
    return -1;
}

caddr_t _sbrk(int increment) {
    if (increment == 0) {
        return (void*)atomic_load(&__heap_end);
    }

    uintptr_t old_end, new_end;

    do {
        old_end = atomic_load(&__heap_end);
        new_end = old_end + increment;

        // align...
        const uintptr_t aligned_start = old_end & ~(0xFFF);
        const uintptr_t aligned_end   = (new_end + 0xFFF) & ~(0xFFF);

        if (aligned_end > aligned_start) {
            // TODO we don't always need to do this. It's basically no-op to recreate,
            //      so not the end of the world, but should be tidied up. 
            //
            //      sbrk is often called with increments that are significantly
            //      smaller than page-size, which don't require a region / will
            //      recreate the same region multiple times...
            //
            if (anos_create_region(aligned_start, aligned_end, REGION_FLAG_AUTOMAP) < 0) {
                errno = ENOMEM;
                return (void*)-1;
            }
        }
    } while (!atomic_compare_exchange_weak(&__heap_end, &old_end, new_end));

    return (void*)old_end;
}

int _stat(const char *file, struct stat *st) {
    errno = EBADF;
    return -1;
}

clock_t _times(struct tms *buf) {
    buf->tms_utime = 0;
    buf->tms_stime = 0;
    buf->tms_cstime = 0;
    buf->tms_cutime = 0;

    return buf->tms_utime;
}

int _unlink(char *name) {
    errno = ENOENT;
    return -1;
}

int _wait(int *status) {
    errno = ECHILD;
    return -1;
}

int _write(int file, char *ptr, int len) {
    errno = ENOENT;
    return -1;
}

int _gettimeofday(struct timeval *p, struct timezone *z) {
    errno = ENOSYS;
    return -1;
}

void __attribute__((weak)) breakpoint() { 
    // Weak empty breakpoint for when mgdb is not specified...
}