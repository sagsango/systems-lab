#ifndef __CUSTOM_ALLOCATOR
#define __CUSTOM_ALLOCATOR
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void init_allocator();
int Free(char*);
char *Malloc(unsigned int);
void chunkTraveler();
#endif /* __CUSTOM_ALLOCATOR */
