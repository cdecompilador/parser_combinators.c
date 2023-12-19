#ifndef ERROR_H
#define ERROR_H

#include "fwd.h"

struct Error
{
    showable (*const to_showable)(const void*);
    void (*const _destroy)(const void*);
};

struct error_like
{
    void *self;
    Error const *vtable;
};

#endif
