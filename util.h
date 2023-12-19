#ifndef UTIL_H
#define UTIL_H

#include "fwd.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t
#define u64 uint64_t

#define i8 int8_t
#define i16 int16_t
#define i32 int32_t
#define i64 int64_t

#define usize size_t

#define len(x) _Generic((x), \
    string_buffer*: string_buffer_len \
    )(x)

#define idx(s, x) _Generic((s), \
    string_buffer*: string_buffer_idx \
    )(s, x)

/* Swap two refereces of value T */
#define SWAP(T, a_ref, b_ref) {\
        ASSERT_NON_ZERO(a_ref);\
        ASSERT_NON_ZERO(b_ref);\
        static_assert(sizeof(*a_ref) == sizeof(*b_ref),\
                "In SWAP macro types of the operands differ");\
        T tmp = *(T*)a_ref;\
        *(T*)a_ref = *(T*)b_ref;\
        *(T*)b_ref = tmp;\
    }

/* Implement an interface to a type
 *  - I_type: the interface name
 *  - I_instance_type: the interface instance type
 *  - self_type: the type for which the interface is being implemented
 *  - block: the list of functions the do the implementation of the interface
 *
 *  For example:
 *  
 *    struct foo;
 *
 *    foo_show() { return "foo"; }
 *
 *    IMPL_INTERFACE(Show, showable, foo, { . show = foo_show });
 *
 *  Now tpye `foo` can be used for things like:
 *
 *      foo f;
 *      PRINT(foo, &f);
 *
 * Internally defines the <Type>_to_<InterfaceInstance> function and makes the
 * assignment of the vtable easier avoiding a lot of code duplication, 
 */
#define IMPL_INTERFACE(I_type, I_instance_type, self_type, block)\
    I_instance_type\
    self_type##_to_##I_instance_type(self_type *self)\
    {\
        static I_type self_type##_##I_type##_vtable = (I_type) block;\
        \
        return (I_instance_type) {\
            .self = (void*)self,\
            .vtable = &self_type##_##I_type##_vtable\
        };\
    }

#ifndef ENABLE_DEBUG_ASSERTIONS
#define ENABLE_DEBUG_ASSERTIONS 1
#endif

#ifndef ASSERT
#define ASSERT(cond, ...) if (!(cond))\
    _assert_failed(#cond, __FILE__, __LINE__, __VA_ARGS__);
#endif

#define ASSERT_NON_ZERO(value) ASSERT(value, "'%s' must be a non-zero value", #value)
#define ASSERT_BOUNDS(value, min, max)\
    ASSERT(value >= min && value <= max, "'%s' is OOB of range [%s, %s]", #value, #min, #max)

void
_assert_failed(
    const char *cond,
    const char *file, i32 line,
    const char *message, ...);

#define OK(T, _value) (T) { .success = true, .value = (_value) }
#define ERR(T, _error) (T) { .success = false, .error = (_error) }

#define RES_CHECK(T, _result) ({\
        typeof(_result) res = (_result);\
        if (!res.success) {\
            return (T) { .success = false, .error = res.error };\
        } else {\
            /* _clear_backtrace(); */\
        }\
        res.value; })

/* Get the inner value of the `result_*` and exit program in case its impossible 
 */
#define UNWRAP(_result) ({\
        typeof(_result) res = (_result);\
        if (!res.success) {\
            /* error_describe(res.error);\ */\
            printf("Unhandled unwrap on %s:%d\n", __FILE__, __LINE__);\
            /* print_backtrace(); */\
            exit(1);\
        } else {\
            /* _clear_backtrace(); */\
        }\
        res.value; })

#define TYPEDEF_RESULT(T) struct result_##T {\
    bool success;\
    union {\
        error_like error;\
        T value;\
    };\
}; typedef struct result_##T result_##T;

/*
struct result_void {
    bool success;
    union {
        error_like error;
        void *value;
    };
};

TYPEDEF_RESULT(u8);
TYPEDEF_RESULT(u16);
TYPEDEF_RESULT(u32);
TYPEDEF_RESULT(u64);
TYPEDEF_RESULT(usize);

TYPEDEF_RESULT(i8);
TYPEDEF_RESULT(i16);
TYPEDEF_RESULT(i32);
TYPEDEF_RESULT(i64);

*/

#endif
