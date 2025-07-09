/*
 *
 * Defer implementation
 *
 * syntac is:
 *      defer({
 *          free(ptr1);
 *          free(ptr2);
 *      });
 *
 *  Notes:
 *  - Need to define __LSP__ in a .clangd file to stop the errors
 *  - The LSP may not see the code errors in the defer block, so keep it simple
 */

#ifndef DEFER_H
#define DEFER_H

#include <stdlib.h>

// Define a cleanup function type
typedef void (*defer_cleanup_fn)(void*);

// Structure to hold cleanup info
typedef struct {
    defer_cleanup_fn cleanup;
    void* context;
} defer_context_t;

// The cleanup function that gets called
static inline void defer_execute(defer_context_t* ctx) {
    if (ctx && ctx->cleanup) {
        ctx->cleanup(ctx->context);
    }
}

// Helper macro to create unique names
#define DEFER_CONCAT_(a, b) a##b
#define DEFER_CONCAT(a, b) DEFER_CONCAT_(a, b)

// The main defer macro
#ifdef __LSP__
// Simplified version for LSP analysis that's easier to parse
#define defer(code) \
    do {            \
    } while (0)
#elif defined(__GNUC__) || defined(__clang__)
#define defer(code)                                                       \
    void DEFER_CONCAT(__defer_cleanup_,                                   \
                      __LINE__)(void* context __attribute__((unused))) {  \
        code;                                                             \
    }                                                                     \
    __attribute__((cleanup(defer_execute))) defer_context_t DEFER_CONCAT( \
        __defer_ctx_, __LINE__) = {                                       \
        .cleanup = DEFER_CONCAT(__defer_cleanup_, __LINE__), .context = NULL}
#elif defined(_MSC_VER)
// MSVC doesn't support cleanup attributes, but we can use a workaround with
// __try/__finally
#define defer(code)                                   \
    struct {                                          \
        int dummy;                                    \
    } DEFER_CONCAT(__defer_dummy_, __LINE__);         \
    __try {                                           \
        (void)DEFER_CONCAT(__defer_dummy_, __LINE__); \
    } __finally {                                     \
        code;                                         \
    }
#else
#error "defer is not supported on this compiler"
#endif

#endif  // DEFER_H
