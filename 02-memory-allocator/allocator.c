#include <stdio.h>
#include "allocator.h"

#define ALLOCATOR_SIZE (1<<20)

char buf[ALLOCATOR_SIZE];

/*
   Free blocks
   Used We dont have to keep track
*/

/*
   minimum size = 8 Bytes + 8 Bytes = 16 Bytes
   alignment = 8 Bytes
 */

/*
  TODO: Things to be casefull about:
    1. Pointer arithmatic;
      char * ptr; ptr += 1; will be incremented by 1 Byte
      struct stuff * ptr; ptr += 1; will be incremented by sizeof(struct stuff)
    2. Sentinal nodes
    3. At worst you have to merge curretny freeing chunk with its 2 negibors
       so O(1)
    4. User will work on data of the chunk so we have to return data ptr not
  header And during free we will calculate the header again
*/

#define HEADER_SIZE 16
#define ALIGN 8

struct chunk {
  struct chunk * next; 
  unsigned long tot_size; /* header + data */
};


/* TODO: Polulate it */
struct allocator_stats{
  int free_mem;
  int used_mem;
  int free_chunks;
  int used_chunks;
};

struct allocator_stats st;  

void init_allocator() {
    
    struct chunk * ptr = (struct chunk*)buf;
    /* Allocate a sentian chunk at the begining */
    ptr->next = (struct chunk *)(buf + HEADER_SIZE);
    ptr->tot_size = 0;

    ptr = ptr->next;
    ptr->next = NULL;
    ptr->tot_size = ALLOCATOR_SIZE - HEADER_SIZE;

}


char * Malloc(unsigned int size) {
  if (size == 0) {
    return NULL;
  }

  size += HEADER_SIZE;
  size = (size + 8 - 1) & ~(0b111); /* 8 byte aligned */

  struct chunk * ptr = (struct chunk*)buf; /* This will always be a free Node */
  struct chunk * prv = NULL;

  while (ptr) {
    if (ptr->tot_size >= size) {
      break;
    }
    prv = ptr;
    ptr = ptr->next;
  }

  if (!ptr) {
    return NULL;
  }

  if (ptr->tot_size > size + HEADER_SIZE) {
    /* Current chunk is too big!! */
    ptr->tot_size -= size;
    ptr = ((char*)ptr + ptr->tot_size);

    ptr->next = NULL;
    ptr->tot_size = size;
  }
  else {
    /* Do not cut the chunk */
    prv->next = ptr->next;
    ptr->next = NULL;
  }
  return ((char*)ptr + HEADER_SIZE);
}


int Free(char *p) {
  if (!p) {
    return 1;
  }

  struct chunk * c = (struct chunk*) (p - HEADER_SIZE);
  struct chunk * ptr = (struct chunk *)buf;
  struct chunk * prv = NULL;

  while (ptr && ptr < c) {
    prv = ptr;
    ptr = ptr->next;
  }

  /* prv will always be non NULL because of sentinal chunk */
  if ((char*)prv + prv->tot_size == c) { /*XXX: Pointer arithmatic so we have to typecast it to char * */
    prv->tot_size += c->tot_size;
    c = prv;
  } else {
    c->next = prv->next;
    prv->next = c;
  }

  if (c->next && (char*)c + c->tot_size == c->next) {
    c->tot_size += c->next->tot_size;
    c->next = c->next->next;
  }
  return 0;
}

void chunkTraveler() {
  struct chunk * ptr = (struct chunk*)buf;
  while (ptr) {
    printf("%lld[next:%lld, size:%lld]->", (long long)ptr, (long long)ptr->next, (long long)ptr->tot_size);
		ptr = ptr->next;
  }
  printf("NULL\n");
}
