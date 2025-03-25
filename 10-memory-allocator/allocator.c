#include <stdio.h>
#include "allocator.h"
#include <assert.h>

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

#define HEADER_SIZE 16
#define ALIGN 8
#define SENTINAL -1

struct chunk {
  struct chunk * next; 
  long tot_size; /* header + data */
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
    ptr->tot_size = SENTINAL;

    ptr = ptr->next;
    ptr->next = (struct chunk *)(buf + ALLOCATOR_SIZE - HEADER_SIZE);
    ptr->tot_size = ALLOCATOR_SIZE - HEADER_SIZE - HEADER_SIZE;

    /* Allocate a sentian chunk at the end */
    ptr = ptr->next;
    ptr->next = NULL;
    ptr->tot_size = SENTINAL;
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

  struct chunk * cur = (struct chunk*) (p - HEADER_SIZE);
  struct chunk * nxt = (struct chunk *)buf;
  struct chunk * prv = NULL;

  while (nxt < cur) {
    prv = nxt;
    nxt = nxt->next;
  }

  /* 
  ** prv  & nxt will always be non NULL because of sentinal chunk
  ** When we are merging  the cur with prv & nxt
  ** We are doing pointer + int arithmatic; which be avoid sentianl nodes to be coalased
  ** But still we have made a check. Which is more readable
  **
  */

  /* cur & prv*/
  if (prv->tot_size != SENTINAL &&  (char*)prv + prv->tot_size == cur) { /*XXX: Pointer arithmatic so we have to typecast it to char * */
    prv->tot_size += cur->tot_size;
    cur = prv;
  } else {
    assert (nxt == prv->next);
    cur->next = nxt;
    prv->next = cur;
  }

  /* cur & next*/
  if (nxt->tot_size != SENTINAL && (char*)cur + cur->tot_size == nxt) {
    cur->tot_size += nxt->tot_size;
    cur->next = nxt->next;
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
