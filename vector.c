#include "vector.h"

#include <stdlib.h>

byte_slice
byte_slice_from(u8 *ptr, usize len)
{
    return (byte_slice) {
        .ptr = ptr,
        .len = len
    };
}

byte_vector
byte_vector_create(usize initial_capacity)
{
    u8 *data;

    ASSERT_NON_ZERO(initial_capacity);

    data = (u8*)malloc(initial_capacity);

    ASSERT_NON_ZERO(data);

    return (byte_vector) {
        .data = data,
        .cap = initial_capacity,
        .count = 0
    };
}

void
byte_vector_destroy(byte_vector *self)
{
    ASSERT_NON_ZERO(self);

    free(self->data);
}

byte_vector
byte_vector_clone(byte_vector *self)
{
    ASSERT_NON_ZERO(self);

    u8* new_data = malloc(self->cap);
    ASSERT_NON_ZERO(new_data);

    ASSERT_NON_ZERO(memcpy(new_data, self->data, self->count));

    return (byte_vector) {
        .data = new_data,
        .count = self->count,
        .cap = self->cap
    };
}

void
byte_vector_reserve(byte_vector *self, usize extra_size)
{
    ASSERT_NON_ZERO(self);
    ASSERT_NON_ZERO(self->data);

    self->cap += extra_size;
    self->data = realloc(self->data, self->cap);

    ASSERT_NON_ZERO(self->data);
}

void
byte_vector_push(byte_vector *self, u8 byte)
{
    u8 *ptr;

    ASSERT_NON_ZERO(self);

    /* Check if its needed to extend the allocated data capacity */
    if (self->count == self->cap) {
        self->cap *= 2;
        u8 *data = realloc(self->data, self->cap);

        ASSERT_NON_ZERO(data);

        self->data = data;
    }

    /* Push data to the end */
    ptr = (u8*)self->data;
    ptr += self->count;
    self->count++;
    *ptr = byte;
}

/* TODO: Add something like `push_front_n` since this is very inefficient for
 * large prepends */
void
byte_vector_push_front(byte_vector *self, u8 byte)
{
    ASSERT_NON_ZERO(self);

    /* Check for available space */
    if (self->count + 1 >= self->cap) {
        self->cap *= 2;
        u8 *data = realloc(self->data, self->cap);

        ASSERT_NON_ZERO(data);

        self->data = data;
    }

#include <stdio.h>
    /* Shift all characters to the right by 1 */
    for (int i = self->count; i >= 0; i--) 
    {
        *(self->data + i) = *(self->data + i - 1);   
    }
    self->count++;

    /* Prepend */
    *self->data = byte;
}

void
byte_vector_write_from(byte_vector *self, byte_slice src_slice)
{
    u8 *data;

    ASSERT_NON_ZERO(self);
    ASSERT_VALID_SLICE(src_slice);

    /* Realloc until the data buffer is big enough */
    while (self->cap < self->count + src_slice.len) {
        self->cap *= 2;
        data = realloc(self->data, self->cap);

        ASSERT_NON_ZERO(data);

        self->data = data;
    }

    /* Do the memcpy */
    memcpy(self->data + self->count, src_slice.ptr, src_slice.len);
    self->count += src_slice.len;
}
