#include "string_buffer.h"

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

#include "util.h"

string_buffer
string_buffer_empty()
{
    byte_vector bv = byte_vector_create(8);
    return (string_buffer) {
        .byte_vector = bv
    };
}

string_buffer
string_buffer_create(const char *str)
{
    ASSERT_NON_ZERO(str);

    string_buffer sb = string_buffer_empty();

    string_buffer_push_str(&sb, str);

    return sb;
}

void
string_buffer_destroy(string_buffer *self)
{
    ASSERT_NON_ZERO(self);

    byte_vector_destroy(&self->byte_vector);
}

string_buffer
string_buffer_clone(string_buffer *self)
{
    ASSERT_NON_ZERO(self);

    return (string_buffer) {
        byte_vector_clone(&self->byte_vector)
    };
}

string_view
string_buffer_as_slice(string_buffer* self)
{
    const u8* ptr = self->byte_vector.data;
    usize len = self->byte_vector.count;

    return (string_view) {
        .ptr = (const char*)ptr,
        .len = len
    };
}

void
string_buffer_push_str(string_buffer *self, const char *str)
{
    usize len;

    ASSERT_NON_ZERO(self);
    ASSERT_NON_ZERO(str);

    len = strlen(str);

    if (len == 0)
        return;

    byte_vector_write_from(
        &self->byte_vector,
        byte_slice_from((u8*)str, len));
}

void
string_buffer_push_chr(string_buffer *self, char chr)
{
    ASSERT_NON_ZERO(self);
    ASSERT_NON_ZERO(chr);

    byte_vector_push(&self->byte_vector, chr);
}

void
string_buffer_push(string_buffer* self, string_view view)
{
    ASSERT_NON_ZERO(self);
    ASSERT_VALID_SLICE(view);

    byte_vector_write_from(
        &self->byte_vector, byte_slice_from((u8*)view.ptr, view.len));
}

void
string_buffer_push_format(string_buffer *self, const char *format, ...)
{
    va_list args;
    u8 *buf_ptr;
    int len;

    ASSERT_NON_ZERO(self);
    ASSERT_NON_ZERO(format);

    va_start(args, format);

    buf_ptr = self->byte_vector.data + self->byte_vector.count;

    /* TODO: Improve to support >64 characters */
    byte_vector_reserve(&self->byte_vector, 64);
    len = vsnprintf((char*)buf_ptr, 64, format, args);

    ASSERT(len >= 0, "vsnprintf failed");

    self->byte_vector.count += len;

    va_end(args);
}

/* Prepend a string to the buffer */
void
string_buffer_prepend_str(string_buffer *self, const char *str)
{
    ASSERT_NON_ZERO(self);
    ASSERT_NON_ZERO(str);

    for (int i = 0; i < strlen(str); i++) {
        byte_vector_push_front(&self->byte_vector, *(str + i));
    }
}

char*
string_buffer_idx(string_buffer *self, int i)
{
    return self->byte_vector.data + i;
}

int
string_buffer_len(string_buffer *self)
{
    return self->byte_vector.count;
}

void
string_buffer_reverse(string_buffer *self)
{
    ASSERT_NON_ZERO(self);

    for (int i = 0; i < len(self) / 2; i++)
    {
        SWAP(
            char,
            idx(self, i),
            idx(self, len(self) - i - 1)
        );
    }
}
