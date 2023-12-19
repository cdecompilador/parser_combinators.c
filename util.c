#include "util.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

void
_assert_failed(
    const char *cond,
    const char *file, i32 line,
    const char *message, ...)
{
    va_list va_args;

    fprintf(stderr,"assertion failed: %s\nmessage: ", cond);

    va_start(va_args, message);
    vfprintf(stderr, message, va_args);
    va_end(va_args);

    fprintf(stderr, "\nfile: %s:%u\n", file, line);

    // _print_backtrace();

    abort();
}
