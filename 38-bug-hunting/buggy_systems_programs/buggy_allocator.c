#include <unistd.h>
#include <stdio.h>
#include <string.h>

typedef struct Block {
    size_t size;
    struct Block *next;
    int free;
    char data[1];
} Block;

#define BLOCK_SIZE sizeof(Block)

Block *head = NULL;

void *my_malloc(size_t size) {
    Block *curr = head;
    while (curr) {
        if (curr->free && curr->size >= size) {
            curr->free = 0;
            return curr->data;
        }
        curr = curr->next;
    }

    Block *new_block = sbrk(size + BLOCK_SIZE); // 🔴 No sbrk error check
    new_block->size = size;
    new_block->free = 0;
    new_block->next = head;
    head = new_block;
    return new_block->data;
}

void my_free(void *ptr) {
    if (!ptr) return;
    Block *block = (Block *)((char *)ptr - BLOCK_SIZE);
    block->free = 1; // 🔴 No coalescing or reuse check
}

int main() {
    char *a = my_malloc(20);
    strcpy(a, "hello");
    my_free(a);
    my_free(a); // 🔴 Double free
    return 0;
}