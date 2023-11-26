#ifndef __POOL_H__
#define __POOL_H__
// Inline functions, #defines and includes that will be
// needed for all instantiations can go up here.
#include <stdlib.h>		// realloc, size_t

#include "dump.h"

#define POOL_IMPL(word) POOL_COMB1(POOL_PREFIX,word)
#define POOL_COMB1(pre, word) POOL_COMB2(pre, word)
#define POOL_COMB2(pre, word) pre##word

#endif				// __POOL_H__

#ifndef POOL_T
typedef void *opaque;
#define POOL_T opaque
#endif

// The name of the data type to be generated.
// If not given, will expand to something like
// `POOL_int` for an `int`.
#ifndef POOL_NAME
#define POOL_NAME POOL_COMB1(POOL_COMB1(POOL,_), POOL_T)
#endif

// Prefix for generated functions.
#ifndef POOL_PREFIX
#define POOL_PREFIX POOL_COMB1(POOL_NAME, _)
#endif

// Customize the linkage of the function.
#ifndef POOL_LINKAGE
#ifndef POOL_DECLS_ONLY
#define POOL_LINKAGE static inline
#else
#define POOL_LINKAGE
#endif
#endif

typedef struct POOL_NAME POOL_NAME;
struct POOL_NAME {
    POOL_T *items;
    size_t count;
    size_t capacity;
};

#define POOL_add POOL_IMPL(add)
#define POOL_del POOL_IMPL(del)
#define POOL_get POOL_IMPL(get)
#define POOL_len POOL_IMPL(len)
#define POOL_resize POOL_IMPL(resize)
#define POOL_dump POOL_IMPL(dump)

POOL_LINKAGE void POOL_add(POOL_NAME * array, POOL_T item);

POOL_LINKAGE void POOL_del(POOL_NAME * array, int idx);

POOL_LINKAGE POOL_T * POOL_get(POOL_NAME * array, int idx);

POOL_LINKAGE size_t POOL_len(POOL_NAME * array);

POOL_LINKAGE void POOL_dump(POOL_NAME * array);

#ifndef POOL_DECLS_ONLY
static inline size_t max(size_t a, size_t b, size_t c)
{
    size_t max_ab;
    size_t max_abc;

    max_ab = a > b ? a : b;
    max_abc = max_ab > c ? max_ab : c;

    return max_abc;
}

static inline void POOL_resize(POOL_NAME * array, size_t count)
{
    if (array->capacity >= count)
	return;

    size_t old_cap = array->capacity;
    size_t new_cap = max(4, count, old_cap * 2);
    size_t new_size = new_cap * sizeof(POOL_T);
    array->items = realloc(array->items, new_size);
    array->capacity = new_cap;
}

POOL_LINKAGE void POOL_add(POOL_NAME * array, POOL_T item)
{
    POOL_resize(array, array->count + 1);
    array->items[array->count] = item;
    array->count++;
}

POOL_LINKAGE void POOL_del(POOL_NAME * array, int idx)
{
    // Copy the last item in place to compress the pool
    array->items[idx] = array->items[array->count - 1];
    array->count--;
}

POOL_LINKAGE POOL_T * POOL_get(POOL_NAME * array, int idx)
{
    return array->items + idx;
}

POOL_LINKAGE size_t POOL_len(POOL_NAME * array)
{
    return array->count;
}

POOL_LINKAGE void POOL_dump(POOL_NAME * array) {
    hex_dump("pool", array, sizeof(POOL_NAME), 16);
    hex_dump("pool->items", array->items, array->capacity * sizeof(POOL_T), 16);
}

#elif
#undef POOL_DECLS_ONLY
#endif

// Cleanup
// These need to be undef'ed so they can be redefined the
// next time you need to instantiate this template.
#undef POOL_T
#undef POOL_PREFIX
#undef POOL_NAME
#undef POOL_LINKAGE
#undef POOL_push


