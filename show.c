#include "show.h"

#include "string_buffer.h"

string_buffer
show(showable s)
{
    ASSERT_NON_ZERO(s.self);
    ASSERT_NON_ZERO(s.vtable);
    ASSERT_NON_ZERO(s.vtable->show);

    return s.vtable->show(s.self);
}

struct_formatter
fmt_struct(const char *struct_name)
{
    struct_formatter sf;

    ASSERT_NON_ZERO(struct_name);

    sf = (struct_formatter) {
        string_buffer_create(struct_name)
    };
    string_buffer_push_str(&sf.buf, " {\n");

    return sf;
}

void
fmt_struct_add_field(struct_formatter *self, 
        const char *field_name, string_buffer field_show)
{
    ASSERT_NON_ZERO(self);
    ASSERT_NON_ZERO(field_name);

    string_buffer_push_str(&self->buf, "\t");
    string_buffer_push_str(&self->buf, field_name);
    string_buffer_push_str(&self->buf, ": ");
    string_buffer_push(&self->buf, string_buffer_as_slice(&field_show));
    string_buffer_push_str(&self->buf, "\n");

    string_buffer_destroy(&field_show);
}

string_buffer
fmt_struct_finish(struct_formatter *self)
{
    ASSERT_NON_ZERO(self);

    string_buffer_push_str(&self->buf, "}");

    return self->buf;
}

showable
error_to_showable(error_like err)
{
    ASSERT_NON_ZERO(err.self);
    ASSERT_NON_ZERO(err.vtable);
    ASSERT_NON_ZERO(err.vtable->to_showable);

    return err.vtable->to_showable(err.self);
}

string_buffer
i32_show(void* _self)
{
    i32 self, tmp;
    bool was_negative = false;
    string_buffer res;

    self = (i32)(u64)_self;

    res = string_buffer_empty();

    if (self < 0) {
        was_negative = true;
        self = -self;
    }

    while (self != 0) 
    {
        tmp = self % 10;
        string_buffer_push_chr(&res, (char)(i32)(tmp + '0'));
        self /= 10;
    }

    if (was_negative) {
        string_buffer_push_chr(&res, '-');
    }

    string_buffer_reverse(&res);

    return res;
}

showable
i32_to_showable(i32 value)
{
    static Show i32_show_vtable = (Show) {
        .show = i32_show
    };

    return (showable) {
        .self = (void*)(i64)value,
        .vtable = &i32_show_vtable
    };
}
