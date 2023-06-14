#ifndef http11_common_h
#define http11_common_h

#include <cstddef>
#include <cstdint>

typedef void (*element_cb)(void *data, const char *at, size_t length);
typedef void (*field_cb)(void *data, const char *field, size_t flen, const char *value, size_t vlen);

#endif