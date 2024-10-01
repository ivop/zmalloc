# zmalloc

## Dynamic Memory Allocation for Small Memory Systems

ZMALLOC is meant for systems with a relatively small amount of memory,
mostly bare-metal, single process systems, like old micro computers
or embedded systems, with a known fixed amount of memory which you
initialize through zmalloc_init at the beginning of your program.

## Design

Overhead per allocated block is just sizeof(uintptr_t) bytes.

- Assume sizeof(uintptr_t) == sizeof(void*).
- Alignment is always to sizeof(uintptr_t) bytes.
- Allocated block sizes are always a multiple of alignment, with a minimum
  of two times the size of a pointer (needed when the block becomes free).
- Size field uses bit 0 to indicate free (0) or in use (1).
- The sentinels are always part of free_list but will never be allocated
  because their size is zero (this avoids several pointer checks).
- All blocks (used and free) can always be traversed, starting from the
  base, going in forward direction.
- Coalescing with next block is fast.
- If coalesced with next block, previous block in free_list is known and
  can be checked to be adjacent, and if so, merged.
- If not coalesced with next block, search next free block upwards.
  Check if previous block in free_list is adjacent. If so, merge.
  If not, insert new free block in between.
