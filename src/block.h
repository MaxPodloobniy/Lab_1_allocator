#include <stdbool.h>
#include <stddef.h>
#include "allocator_impl.h"
#include "tree.h"

struct block {
    size_t size_curr;
    size_t size_prev;
    size_t offset;
    bool flag_busy;
    bool flag_first;
    bool flag_last;
};

#define BLOCK_STRUCT_SIZE ROUND_BYTES(sizeof(struct block))
#define BLOCK_SIZE_MIN ROUND_BYTES(sizeof(tree_node_type))

struct block *block_split(struct block *, size_t);
void block_merge(struct block *, struct block *);
void block_dontneed(struct block *);

static inline void *
block_to_payload(const struct block *block)
{
    return (char *)block + BLOCK_STRUCT_SIZE;
}

static inline struct block *
payload_to_block(const void *ptr)
{
    return (struct block *)((char *)ptr - BLOCK_STRUCT_SIZE);
}

static inline tree_node_type *
block_to_node(const struct block *block)
{
    return block_to_payload(block);
}

static inline struct block *
node_to_block(const tree_node_type *node)
{
    return payload_to_block(node);
}

static inline void
block_set_size_curr(struct block *block, size_t size)
{
    block->size_curr = size;
}

static inline size_t
block_get_size_curr(const struct block *block)
{
    return block->size_curr;
}

static inline void
block_set_size_prev(struct block *block, size_t size)
{
    block->size_prev = size;
}

static inline size_t
block_get_size_prev(const struct block *block)
{
    return block->size_prev;
}

static inline void
block_set_flag_busy(struct block *block)
{
    block->flag_busy = true;
}

static inline bool
block_get_flag_busy(const struct block *block)
{
    return block->flag_busy;
}

static inline void
block_clr_flag_busy(struct block *block)
{
    block->flag_busy = false;
}

static inline void
block_set_flag_first(struct block *block)
{
    block->flag_first = true;
}

static inline bool
block_get_flag_first(const struct block *block)
{
    return block->flag_first;
}

static inline void
block_set_flag_last(struct block *block)
{
    block->flag_last = true;
}

static inline bool
block_get_flag_last(const struct block *block)
{
    return block->flag_last;
}

static inline void
block_clr_flag_last(struct block *block)
{
    block->flag_last = false;
}

static inline void
block_set_offset(struct block *block, size_t offset)
{
    block->offset = offset;
}

static inline size_t
block_get_offset(const struct block *block)
{
    return block->offset;
}

static inline struct block *
block_next(const struct block *block)
{
    return (struct block *)
        ((char *)block + BLOCK_STRUCT_SIZE + block_get_size_curr(block));
}

static inline struct block *
block_prev(const struct block *block)
{
    return (struct block *)
        ((char *)block - BLOCK_STRUCT_SIZE - block_get_size_prev(block));
}

static inline bool
is_busy(struct block *block)
{
    return block->flag_busy;
}

static inline void
arena_init(struct block *block, size_t size)
{
    block->size_curr = size;
    block->size_prev = 0;
    block->offset = 0;
    block->flag_busy = false;
    block->flag_first = true;
    block->flag_last = true;
}

static inline void
block_init(struct block *block)
{
    block->flag_busy = false;
    block->flag_first = false;
    block->flag_last = false;
}


