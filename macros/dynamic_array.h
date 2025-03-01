// TODO: CHECK THESE

/*
 *
 * Dynamic array implementation
 *
 * Need to define a struct type of
 * typedef struct {
 *   void* items;
 *   size_t count;
 *   size_t capacity;
 * } da_t;
 *
 * 'void' can be any type
 */

#define DA_INIT_CAP 256

#define da_get(da, index) ((index) < (da)->count ? (da)->items[index] : 0)

#define da_append(da, item)                                                    \
    do {                                                                       \
        if ((da)->count >= (da)->capacity) {                                   \
            (da)->capacity =                                                   \
                (da)->capacity == 0 ? DA_INIT_CAP : (da)->capacity * 2;        \
            (da)->items =                                                      \
                realloc((da)->items, (da)->capacity * sizeof(*(da)->items));   \
            assert((da)->items != NULL && "Realloc failed");                   \
        }                                                                      \
        (da)->items[(da)->count++] = (item);                                   \
    } while (0)

#define da_append_many(da, new_items, new_items_count)                         \
    do {                                                                       \
        if ((da)->count + (new_items_count) > (da)->capacity) {                \
            while ((da)->count + (new_items_count) > (da)->capacity) {         \
                (da)->capacity =                                               \
                    (da)->capacity == 0 ? DA_INIT_CAP : (da)->capacity * 2;    \
            }                                                                  \
            (da)->items =                                                      \
                realloc((da)->items, (da)->capacity * sizeof(*(da)->items));   \
            assert((da)->items != NULL && "Realloc failed");                   \
        }                                                                      \
        memcpy((da)->items + (da)->count, new_items,                           \
               (new_items_count) * sizeof(*(da)->items));                      \
        (da)->count += (new_items_count);                                      \
    } while (0)

#define da_remove(da, index)                                                   \
    do {                                                                       \
        assert((index) < (da)->count && "Index out of bounds");                \
        memmove((da)->items + (index), (da)->items + (index) + 1,              \
                ((da)->count - (index) - 1) * sizeof(*(da)->items));           \
        (da)->count--;                                                         \
    } while (0)

#define da_remove_many(da, remove_start, remove_count)                         \
    do {                                                                       \
        assert((remove_start) + (remove_count) <= (da)->count &&               \
               "Range out of bounds");                                         \
        memmove((da)->items + (remove_start),                                  \
                (da)->items + (remove_start) + (remove_count),                 \
                ((da)->count - (remove_start) - (remove_count)) *              \
                    sizeof(*(da)->items));                                     \
        (da)->count -= (remove_count);                                         \
    } while (0)

#define da_foreach(type, item, da)                                             \
    for (type *item = (da)->items; (item) < (da)->items + (da)->count; ++item)

#define da_resize(da, new_size)                                                \
    do {                                                                       \
        if ((new_size) > (da)->capacity) {                                     \
            (da)->capacity = (new_size);                                       \
            (da)->items = NOB_REALLOC((da)->items,                             \
                                      (da)->capacity * sizeof(*(da)->items));  \
            NOB_ASSERT((da)->items != NULL && "Buy more RAM lol");             \
        }                                                                      \
        (da)->count = (new_size);                                              \
    } while (0)

#define da_free(da)                                                            \
    do {                                                                       \
        free((da)->items);                                                     \
        (da)->items = NULL;                                                    \
        (da)->count = 0;                                                       \
        (da)->capacity = 0;                                                    \
    } while (0)
