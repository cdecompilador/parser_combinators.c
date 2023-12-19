#ifndef STRING_BUFFER_H
#define STRING_BUFFER_H

#include "fwd.h"
#include "util.h"
#include "vector.h"
#include "error.h"

/* Not owned view over a string, it can point to a:
 * - Static reference of c-like string
 * - Reference to a slice on a `string_buffer`
 *
 * # SAFETY
 * Don't use over slices on dynamic c-like strings, there is not way in c to
 * enforce this rule
 */
struct string_view {
    const char *ptr;
    usize len;
};

TYPEDEF_RESULT(string_view);

/* Owned, heap allocated dynamic ASCII string 
 *
 * TODO: Support unicode (just utf-8) */
struct string_buffer {
    byte_vector byte_vector;
};

TYPEDEF_RESULT(string_buffer);

/* Allocate an empty string buffer */
string_buffer
string_buffer_empty();

/* Allocate a string buffer with a value `str` */
string_buffer
string_buffer_create(const char *str);

/* Deallocate string */
void
string_buffer_destroy(string_buffer *self);

string_buffer
string_buffer_clone(string_buffer *self);

/* Get the string view equivalent to all the buffer */
string_view
string_buffer_as_slice(string_buffer *self);

/* Push to the end a c-like string to the buffer */
void
string_buffer_push_str(string_buffer *self, const char *str);

/* Push a string to the buffer */
void
string_buffer_push(string_buffer *self, string_view view);

/* Push a single char to the buffer */
void
string_buffer_push_chr(string_buffer *self, char chr);

/* Push to the string a formated string */
void
string_buffer_push_format(string_buffer *self, const char *format, ...);

/* Prepend a string to the buffer */
void
string_buffer_prepend_str(string_buffer *self, const char *str);

/* Perform an in-place string reverse */
void
string_buffer_reverse(string_buffer *self);

#endif
