#ifndef SHOW_H
#define SHOW_H

#include "fwd.h"

#include "util.h"
#include "error.h"
#include "string_buffer.h"

/* Iterface used to get a human readable representation of data 
 *
 * Don't confuse over `Deserialize` which is intended to get a `string_buffer`
 * in any number of encodings and can fail, this is way simpler you have a 
 * struct and there 1 human readable format */
struct Show
{
    string_buffer (*const show)(void *self);
};

/* Show interface instance */
struct showable
{
    void *self;
    Show const *vtable;
};

#define IMPL_SHOW(T, show_fn)\
    IMPL_INTERFACE(Show, showable, T, { .show = show_fn })

#define SHOW(T, value)\
    show(T##_to_showable(value))

/* Don't ask about the +1 */
#define DBG(T, value) {\
    string_buffer sbuf = SHOW(T, value);\
    string_view sbuf_view = string_buffer_as_slice(&sbuf);\
    printf("[%s:%u] %s = %.*s\n",\
            __FILE__, __LINE__, #value + 1,\
            (int)sbuf_view.len, sbuf_view.ptr);\
    string_buffer_destroy(&sbuf);\
}

#define PRINT(T, value) {\
    string_buffer sbuf = SHOW(T, value);\
    string_view sbuf_view = string_buffer_as_slice(&sbuf);\
    printf("%.*s\n", sbuf_view.len, sbuf_view.ptr);\
    string_buffer_destroy(&sbuf);\
}

/* Easy access to the vtable `show` function pointer with checks */
string_buffer
show(showable s);

/* Syntax sugar for implementing the `Show` interface for a struct, for example
 * if I have the struct:
 *
 *  struct person
 *  {   
 *      i32 age;
 *      i32 height;
 *      string_buffer desc;
 *  };
 *
 * On the interface implementation function `person_show` this can help make
 * the code more readable and safer:
 *
 *  person_show(...)
 *  {
 *      ...
 *
 *      struct_formatter fmt;
 *
 *      fmt = fmt_struct("person");
 *      fmt_struct_add_field(&fmt, "age", SHOW(i32, self->age));
 *      fmt_struct_add_field(&fmt, "height", SHOW(i32, self->height));
 *      fmt_struct_add_field(&fmt, "desc", SHOW(string_buffer, &self->desc));
 *      return fmt_struct_finish(&fmt);
 *  }
 */
struct struct_formatter
{
    string_buffer buf;
};
struct_formatter
fmt_struct(const char *struct_name);
void
fmt_struct_add_field(struct_formatter *self, 
        const char *field_name, string_buffer field_show);
string_buffer
fmt_struct_finish(struct_formatter *self);

/* Default implementation of the `Show` interface for some common types */

showable
error_to_showable(error_like err);

showable
i32_to_showable(i32 value);

#endif
