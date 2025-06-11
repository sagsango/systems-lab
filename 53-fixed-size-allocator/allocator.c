/*
	Apple: Secure Enclave
	Fixed size allocator like kalloc (for page allocation)
	Lets say size is fixed = 256 bytes

	Some probelms and solution
	1. Use after free
		mprotect the pages after free; (better if size = page_size)
		like stack smashing detection; write some bytes when free (and check those bytes during alloc)
	2. double free;
		Heave metadat like inode metadate in  superblock, is inode is beining used or not
*/

#define BLOCK_SIZE 512

/* init the allocator */
void init_allocator(void *memory_pool, size_t size) {
}

/* Returns the block of size BLOCK_SIZE */
void * alloc() {
}

/* Frees the allocated ptr */
void free(void *ptr) {
}
