#ifndef __POOL_H__
#define __POOL_H__

#include <stdlib.h>
#include <string.h>

static void zero_memory(void *s, size_t n)
{
    memset(s, 0, n);
}

struct pool {
    int new_idx;
    char *data; // byte buffer
};

#define POOL_MAX_SIZE (1<<15) // 32k elements per type should be enough for anyone :-p

static void* pool_init(struct pool *pool, size_t pool_element_size)
{
    const struct pool EMPTY_POOL = { 0 };
    *pool = EMPTY_POOL;
    pool->data = calloc(pool_element_size, POOL_MAX_SIZE);
    pool->new_idx = 0;
    return pool->data;
}

static void* pool_new(struct pool *pool, size_t pool_element_size)
{
    void* p = pool->data + pool->new_idx * pool_element_size;
    zero_memory(p, pool_element_size);
    pool->new_idx++;

    return p;
}

static int pool_del(struct pool *pool, size_t pool_element_size, int idx)
{
    pool->new_idx --;

    void* p = pool->data + idx * pool_element_size;
    void* p_last = pool->data + pool->new_idx * pool_element_size;
    memcpy(p, p_last, pool_element_size);

    return idx - 1; // to be able to iterate again
}

#endif // __POOL_H__