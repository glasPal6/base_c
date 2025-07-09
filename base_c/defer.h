#if defined(DEFER_MACRO)  // Must be defined to be able to use it
/* - GCC implementation -
  NOTE: Must use C23 (depending on the platform)
*/
#if defined(COMPILER_GCC)
#define defer __DEFER(__COUNTER__)
#define __DEFER(N) __DEFER_(N)
#define __DEFER_(N) __DEFER__(__DEFER_FUNCTION_##N, __DEFER_VARIABLE_##N)
#define __DEFER__(F, V)        \
    auto void F(int *);        \
    [[gnu::cleanup(F)]] int V; \
    auto void F(int *)

/* - Clang implementation -
  NOTE: Must compile with flag `-fblocks`
*/
#elif defined(COMPILER_CLANG)
typedef void (^const __df_t)(void);

[[maybe_unused]]
static inline void __df_cb(__df_t *__fp) {
    (*__fp)();
}

#define defer __DEFER(__COUNTER__)
#define __DEFER(N) __DEFER_(N)
#define __DEFER_(N) __DEFER__(__DEFER_VARIABLE_##N)
#define __DEFER__(V) [[gnu::cleanup(__df_cb)]] __df_t V = ^void(void)
#endif
#endif
