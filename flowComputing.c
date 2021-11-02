#include <stdio.h>
#include <stdlib.h>

typedef struct Node Node;

struct Node {
	int node_id, decision_id;
	Node* left_child, *right_child;
};

void print_input_file(char*);
Node* create_node(int, Node*, Node*, int);
char** file_to_array(char*);
int get_num_vars(char*);
int get_num_nodes(char*);

int main() {

	char* path = "input_files/bdds/var25.bdd";
	int num_vars = get_num_vars(path);
	int num_nodes = get_num_nodes(path);

	printf("Num vars = %d\nNum nodes = %d\n", num_vars, num_nodes);

	
	return 0;

}

// Implement functions below to complete tasks 1.1 and 1.2

// Function that reads and prints the input file
void print_input_file(char* path) {
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
Node* create_node(int node_id, Node* left_child, Node* right_child, int decision_id) {
	Node* ret = calloc(1, sizeof(Node));
	ret->node_id = node_id;
	ret->left_child = left_child;
	ret->right_child = right_child;
	ret->decision_id = decision_id;
	return ret;
}
 
// Function converts input file into an array of strings
char** file_to_array(char* path) {
	FILE* fptr;
	size_t length = 0;
	ssize_t read;// Variable stores the number of chars read into line
	char* buffer;

	fptr = fopen(path, "r");
	if(fptr == NULL) {
		printf("Error opening File.\n");
		return NULL;
	}
	while ((read = getline(&buffer, &length, fptr)) != -1) {
		printf("Line length = %zu\n", read); // TODO: Remove later
		printf("Buffer length = %zu\n", length); // TODO: Remove later
		printf("%s", buffer);
	}
	fclose(fptr);
	free(buffer);
}

int get_num_vars(char* path) {
	FILE* fptr;
	size_t length = 0;
	char* buffer = NULL;
	ssize_t nread;
	char num_vars_str[3]; // Max num vars being read is double digits

	fptr = fopen(path, "r");
	if(fptr == NULL) {
		printf("Error opening file.\n");
	}

	// Copying input from first line, starting at the 5th element
	nread = getline(&buffer, &length, fptr);
	buffer+=5;
	
	int counter = 0;
	while(*buffer != '\n') {
		num_vars_str[counter++] = *buffer++;
	}
	num_vars_str[counter] = '\0';
	fclose(fptr);
	// Position buffer's ptr back to its original location to free memory correctly
	buffer-=(counter+5);
	free(buffer);
	return atoi(num_vars_str);
}

int get_num_nodes(char* path) {
	FILE* fptr;
	size_t length = 0;
	char* buffer = NULL;
	char num_nodes_str[4]; // Max num nodes being read is triple digits
	ssize_t nread;

	fptr = fopen(path, "r");
	if(fptr == NULL) {
		printf("Error opening file.\n");
	}
	
	// Copying input from second line, starting at the 6th element
	nread = getline(&buffer, &length, fptr);
	nread = getline(&buffer, &length, fptr);
	buffer+=6;
	
	int counter = 0;
	while(*buffer != '\n') {
		num_nodes_str[counter++] = *buffer++;
	}
	num_nodes_str[counter] = '\0';
	fclose(fptr);
	// Position buffer's ptr back to its original location to free memory correctly
	buffer-=(counter+6);
	free(buffer);
	return atoi(num_nodes_str);
}
