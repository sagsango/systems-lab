#include <stdio.h>
#include <stdlib.h>

#define ARR_SIZE 128
int main() {
	char * arr = malloc(sizeof(char) * ARR_SIZE);
	for (int i=0; i<ARR_SIZE; ++i) {
		arr[i] = i % 256;
	}
	free(arr);
	for (int i=0; i<ARR_SIZE; ++i) {
		arr[i] = i % 256;
	}
	free(arr);
}

