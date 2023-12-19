#include <stdio.h>

#include "util.h"
#include "vector.h"
#include "show.h"
#include "string_buffer.h"

inline void
_error_destroy(error_like err)
{
    ASSERT_NON_ZERO(err.self);
    ASSERT_NON_ZERO(err.vtable);
    ASSERT_NON_ZERO(err.vtable->_destroy);

    err.vtable->_destroy(err.self);
}

string_buffer
string_buffer_show(void *_self)
{
    string_buffer self_clone;

    ASSERT_NON_ZERO(_self);

    self_clone = string_buffer_clone((string_buffer*)_self);

    string_buffer_prepend_str(&self_clone, "\"");
    string_buffer_push_str(&self_clone, "\"");

    return self_clone;
}

IMPL_SHOW(string_buffer, string_buffer_show);

struct person
{
    u32 age;
    u32 dni;
    string_buffer desc;
};
typedef struct person person;

string_buffer
person_show(void* _self)
{
    person *self;
    struct_formatter fmt;

    ASSERT_NON_ZERO(_self);

    self = (person*)_self;

    fmt = fmt_struct("person");
    fmt_struct_add_field(&fmt, "dni", SHOW(i32, self->dni));
    fmt_struct_add_field(&fmt, "age", SHOW(i32, self->age));
    fmt_struct_add_field(&fmt, "desc", SHOW(string_buffer, &self->desc));
    return fmt_struct_finish(&fmt);
}

IMPL_SHOW(person, person_show);

/* ------------------------------------------------------------------------- */

int main()
{
    person p;

    p = (person) { 10, 151421, string_buffer_create("hello") };

    DBG(person, &p);

    return 0;
}
