#include <stdio.h>
#include <stdlib.h>

#define INITIAL_CAPACITY 4

typedef struct {
    int *data;
    size_t top;
    size_t capacity;
} Stack;

static int stack_resize(Stack *s) {
    size_t size = s->top + 1;
    size_t new_capacity = s->capacity;

    if (size >= s->capacity) {
        new_capacity = s->capacity * 2;
    }
    else if (s->capacity > INITIAL_CAPACITY &&
             size <= s->capacity / 4) {
        new_capacity = s->capacity / 2;
    }
    else {
        return 0;
    }

    int *new_data = realloc(s->data, new_capacity * sizeof(int));
    if (!new_data)
        return -1;

    s->data = new_data;
    s->capacity = new_capacity;
    return 0;
}

Stack* stack_create() {
    Stack *s = malloc(sizeof(Stack));
    if (!s) return NULL;

    s->capacity = INITIAL_CAPACITY;
    s->top = (size_t)-1;
    s->data = malloc(s->capacity * sizeof(int));
    if (!s->data) {
        free(s);
        return NULL;
    }
    return s;
}

int stack_push(Stack *s, int value) {
    if (stack_resize(s) != 0)
        return -1;

    s->data[++s->top] = value;
    return 0;
}

int stack_pop(Stack *s, int *out) {
    if (s->top == (size_t)-1)
        return -1;

    *out = s->data[s->top--];
    stack_resize(s);

    return 0;
}

void stack_destroy(Stack *s) {
    free(s->data);
    free(s);
}

int main() {
    Stack *s = stack_create();

    for (int i = 0; i < 10; i++)
        stack_push(s, i);

    int x;
    while (stack_pop(s, &x) == 0)
        printf("%d ", x);

    stack_destroy(s);
}

