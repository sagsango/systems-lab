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
	void * ptr = malloc(alignment * 2 + size); /* XXX: why 2 times of alignmnet */
	void * ret;
    size_t rem = ptr & (alignmnet - 1);
	if (rem == 0) {
		ret = ptr;
	} else {
		ret = ptr + (alignment - rem); /* 1st alignmnt may go here */
	}

    /*
     * NOW we are aligned but we need the header
     * so we will put it on the first (alignmnet) size
     * and give then next on to the user
     */
	ret += alignment; /* 2nd alignmnet may go here */

	struct header * h = (char*)ret - HEADER;
	h->malloced_ptr = ptr;

	return ret;
}

void free(void * ptr) {
	struct header * h = (char*)ptr - HEADER;
	free(h->malloced_ptr);
}
