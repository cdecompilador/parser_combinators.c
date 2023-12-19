#ifndef VECTOR_H
#define VECTOR_H

#include "fwd.h"

#include <memory.h>

#include "util.h"

struct byte_slice 
{
    u8 *ptr;
    usize len;
};

byte_slice
byte_slice_from(u8 *ptr, usize len);

#include "error.h"
TYPEDEF_RESULT(byte_slice);

#define ASSERT_VALID_SLICE(value)\
    ASSERT(value.ptr != 0, "'%s' slice invalid", #value)\
    ASSERT(value.len != 0, "'%s' slice invalid len", #value)

/* A fake FIFO dynamic array with stack operations. It does not shrink, it just
 * reduces it's count when `bv_pop` is called */
struct byte_vector 
{
    /* Count of existing elements of the array stack */
    u32 count;

    /* Allocated capacity for the array stack. Always cap >= count */
    u32 cap;

    /* The data */
    u8 *data;
};

/* Create an `byte_vector_t` on the heap and return a pointer to it */
byte_vector
byte_vector_create(usize initial_capacity);

/* Destroy an `byte_vector_t`, removing it from the heap */
void
byte_vector_destroy(byte_vector *self);

byte_vector
byte_vector_clone(byte_vector *self);

/* Reserve some extra size on the byte vector */
void
byte_vector_reserve(byte_vector *self, usize extra_size);

/* Push byte on the stack (can trigger a realloc) */
void
byte_vector_push(byte_vector *self, u8 byte);

void
byte_vector_push_front(byte_vector *self, u8 byte);

void
byte_vector_write_from(byte_vector *self, byte_slice src_slice);

#endif
