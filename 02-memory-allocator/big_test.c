#include <stdio.h>
#include "allocator.h"

const int N = 1<<10;

int main() {
	char * buf[N];
	init_allocator();
	chunkTraveler();
	for (int i=0; i<N; ++i) {
			 buf[i] = Malloc(i+10);
			 if (!buf[i]) {
				 exit(1);
			 }
	}
	for (int i=0; i<N; ++i) {
		if(i&1) {
			Free(buf[i]);
		}
	}
	for (int i=0; i<N; ++i) {
    if(i&1) {
			buf[i] = Malloc(i+10);
			if (!buf[i]) {
				exit(1);
			}
		}
  }
	for (int i=0; i<N; ++i) {
		Free(buf[i]);
	}
	chunkTraveler();
	return 0;
}


