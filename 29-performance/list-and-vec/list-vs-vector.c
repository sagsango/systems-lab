#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 64
#define ITERATIONS 1000000

// Structure for linked list node
struct Node {
    int data;
    struct Node* next;
};

// Function to create a linked list with SIZE elements
struct Node* create_list(int arr[], int size) {
    struct Node* head = NULL;
    struct Node* current = NULL;
    for (int i = 0; i < size; i++) {
        struct Node* new_node = (struct Node*)malloc(sizeof(struct Node));
        new_node->data = arr[i];
        new_node->next = NULL;
        if (head == NULL) {
            head = new_node;
            current = new_node;
        } else {
            current->next = new_node;
            current = new_node;
        }
    }
    return head;
}

// Function to compute sum of vector (array)
long long sum_vector(int vec[], int size, long iterations) {
    long long sum = 0;
    for (long i = 0; i < iterations; i++) {
        for (int j = 0; j < size; j++) {
            sum += vec[j];
        }
    }
    return sum;
}

// Function to compute sum of linked list
long long sum_list(struct Node* head, long iterations) {
    long long sum = 0;
    for (long i = 0; i < iterations; i++) {
        struct Node* current = head;
        while (current != NULL) {
            sum += current->data;
            current = current->next;
        }
    }
    return sum;
}

// Function to free the linked list
void free_list(struct Node* head) {
    struct Node* current = head;
    while (current != NULL) {
        struct Node* temp = current;
        current = current->next;
        free(temp);
    }
}

int main() {
    // Initialize vector and array with same values
    int vector[SIZE];
    for (int i = 0; i < SIZE; i++) {
        vector[i] = i + 1; // Values 1 to 64
    }

    // Create linked list
    struct Node* list = create_list(vector, SIZE);

    // Measure time for vector sum
    clock_t start = clock();
    long long vector_sum = sum_vector(vector, SIZE, ITERATIONS);
    clock_t end = clock();
    double vector_time = ((double)(end - start)) / CLOCKS_PER_SEC;

    // Measure time for list sum
    start = clock();
    long long list_sum = sum_list(list, ITERATIONS);
    end = clock();
    double list_time = ((double)(end - start)) / CLOCKS_PER_SEC;

    // Print results
    printf("Vector sum: %lld, Time: %f seconds\n", vector_sum, vector_time);
    printf("List sum: %lld, Time: %f seconds\n", list_sum, list_time);
    printf("Time difference (List - Vector): %f seconds\n", list_time - vector_time);

    // Free linked list
    free_list(list);

    return 0;
}
