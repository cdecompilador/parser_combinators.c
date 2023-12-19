/* File used for forward declaration and simulated private/public structs, it 
 * asumes you declare your struct in the canonical way:
 *
 * struct T
 * {
 *      ...
 * };
 *
 * This way in case you forgot to add your public structs to this file its 
 * usage in other headers will cause an error (unless you do `struct T`)
 */

/* show.h */
typedef struct showable showable;
typedef struct Show Show;

/* error.h */
typedef struct error_like error_like;
typedef struct Error Error;

/* vector.h */
typedef struct byte_slice byte_slice;
typedef struct byte_vector byte_vector;

/* string_buffer.h */
typedef struct string_view string_view;
typedef struct string_buffer string_buffer;

typedef struct struct_formatter struct_formatter;
