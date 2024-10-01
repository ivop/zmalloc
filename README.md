# *zmalloc*

## Dynamic Memory Allocation for Small Memory Systems

*zmalloc* is a dynamic memory allocation library for systems with a
relatively small amount of memory,
mostly bare-metal, single process systems, like old micro computers
or embedded systems, with a known fixed amount of memory which you
initialize through zmalloc_init at the beginning of your program.

## Algorithm

*zmalloc* maintains a double-linked list of free blocks, called the free list.
Once a block gets allocated, it is removed from the free list and only the size field is maintained, resulting in an overhead of only sizeof(uintptr_t) bytes per allocated memory block.
The blocks in the free list are always ordered by address, and have the following data structure:

```
struct block_info {
    uintptr_t size;
    struct block_info *prev;
    struct block_info *next;
};
```

- Pointers are assumed to be exactly the same size as uintptr_t (sizeof(uintptr_t) == sizeof(void*)).
- Block alignment is always to sizeof(uintptr_t) bytes.
- Allocated block sizes are always rounded up to a multiple of the alignment requirement.
- Size field uses bit 0 to indicate free (0) or in use (1).
- The free list is enclosed by two sentinels which are always part of free_list but will never be allocated
  because their size is set to zero. This avoids several pointer checks, and results in faster code.
- All blocks (used and free) can always be traversed, starting from the base of the memory pool, going in forward direction,
  by continuously adding the block size and the size of the size field.
- Allocation requests are fulfilled by a first-fit policy. If the block found is larger and the remaining area would be big enough
  to hold a free block, the block is split and the remaining area is reinserted into the free list while its order is maintained.
- When a block is freed, its upper neighbouring block is checked whether it's free (and hence part of the free list).
  If it is, both blocks are merged.
- If it is not, the remaining blocks are traversed until a free block (or the ending sentinel) is found.
- Once it is known where to insert the block into the free list, the previous block is checked.
  If it's adjacent to our current free block they are merged.

## Numbers

<table>
    <tr>
        <th></th>
        <th colspan="3"> Pointer size (bits) </th>
    </tr>
    <tr>
        <td></td>
        <td> 16 </td>
        <td> 32 </td>
        <td> 64 </td>
    </tr>
    <tr>
        <th> Alignment (bytes) </th>
        <td> 2 </td>
        <td> 4 </td>
        <td> 8 </td>
    </tr>
    <tr>
        <th> Allocated block overhead (bytes) </th>
        <td> 2 </td>
        <td> 4 </td>
        <td> 8 </td>
    </tr>
    <tr>
        <th> Minimal allocation size (bytes) </th>
        <td> 4 </td>
        <td> 8 </td>
        <td> 16 </td>
    </tr>
</table>
