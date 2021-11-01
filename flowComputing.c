#include <stdio.h>
#include <stdlib.h>

typedef struct Node Node;

struct Node {
	int node_id, decision_id;
	Node* left_child, *right_child;
};

void printInputFile(char*);

int main() {

	char* path = "input_files/bdds/var10.bdd";
	printInputFile(path);

	return 0;

}

// Implement functions below to complete tasks 1.1 and 1.2

// Function that reads and prints the input file
void printInputFile(char* path) {
	FILE* fptr;
	size_t length = 0;
	ssize_t read;// Variable stores the number of chars read into line
	char* buffer;

	fptr = fopen(path, "r");
	if(fptr == NULL) {
		printf("Error opening File.\n");
		return;
	}
	while ((read = getline(&buffer, &length, fptr)) != -1) {
		printf("Line length = %zu\n", read); // TODO: Remove later
		printf("Buffer length = %zu\n", length); // TODO: Remove later
		printf("%s", buffer);
	}
	free(buffer);
}


// Function that creates a new node
// Function that populates each node with its values 
// Function that prints the bdd
