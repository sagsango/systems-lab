#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct header {
	void * malloced_ptr;
};

#define HEADER (sizeof(struct header))
/*
 * alignment >= sizeof(struct header)
 * alignment is a power of 2
 */
void * aligned_malloc(const size_t alignmnet, const size_t size) {
	void * ptr = malloc(alignment * 4 + size);
	void * ret;
	size_t rem = ptr & (alignmnet * 2);
	if (rem == 0) {
		ret = ptr;
	} else {
		ret = ptr + (alignment * 2 - rem);
	}

	ret += alignment;

	struct header * h = (char*)ret - HEADER;
	h->malloced_ptr = ptr;

	return ret;
}

void free(void * ptr) {
	struct header * h = (char*)ptr - HEADER;
	free(h->malloced_ptr);
}
