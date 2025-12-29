#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#ifndef container_of
#define container_of(ptr, type, member) \
    ((type *)((char *)(ptr) - offsetof(type, member)))
#endif

/* LINK: start */
struct link {
    struct link *prev;
    struct link *next;
};


void init_link(struct link *link) {
    link->next = link;
    link->prev = link;
}

void add(struct link *head, struct link *new) {
    new->next = head->next;
    new->prev = head;
    head->next->prev = new;
    head->next = new;
}

void traverse(struct link *head, void (*callback)(struct link *)) {
    struct link *cur = head->next;
    while (cur != head) {
        callback(cur);
        cur = cur->next;
    }
}
/* LINK: end */



/* COLOR: start */
enum color {
    BROWN,
    WHITE,
    GRAY,
    BLACK_N_WHITE
};


char *get_color(enum color c) {
    switch (c) {
        case BROWN: return "brown";
        case WHITE: return "white";
        case GRAY: return "gray";
        case BLACK_N_WHITE: return "black_n_white";
        default: return "unknown";
    }
}
/* COLOR : end */


/* CAT : start */
struct cat {
    char *name;
    enum color c;
    struct link link;
    int age;
};

void print_cat(struct link *link) {
    struct cat *cat = container_of(link, struct cat, link);
    printf("name:%s, color:%s, age:%d\n",
           cat->name, get_color(cat->c), cat->age);
}

struct cat *new_cat(char *name, enum color c, int age) {
    struct cat *cat = malloc(sizeof(*cat));
    if (!cat)
        return NULL;

    cat->name = strdup(name);
    cat->c = c;
    cat->age = age;
    init_link(&cat->link);
    return cat;
}
/* CAT: end */

int main(void) {
    struct link head;
    init_link(&head);

    struct cat *cat;

    cat = new_cat("Brownie", BROWN, 1);
    add(&head, &cat->link);
    traverse(&head, print_cat);

    cat = new_cat("Vanilla", WHITE, 2);
    add(&head, &cat->link);
    traverse(&head, print_cat);

    cat = new_cat("Kitty", GRAY, 2);
    add(&head, &cat->link);
    traverse(&head, print_cat);

    return 0;
}

