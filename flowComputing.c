#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node Node;

struct Node {
	int node_id, decision_id;
	Node* left_child, *right_child;
};

void print_input_file(char*);
Node* create_node(int, Node*, Node*, int);
char** file_to_array(char*, int, int);
int get_num_vars(char*);
int get_num_nodes(char*);
char* test_function();
void free_input_data_arr(char**, int);

int main() {
	char* path = "input_files/bdds/var25.bdd";
	int num_vars = get_num_vars(path);
	int num_nodes = get_num_nodes(path);
	char** input_data_arr = file_to_array(path, num_vars, num_nodes);
	

	free_input_data_arr(input_data_arr, num_vars);	
	return 0;

}

// Function that reads and prints the input file
void print_input_file(char* path) {
	FILE* fptr;
	size_t length = 0; // Buffer's length in bytes
	ssize_t nread; // Variable stores the number of chars read into line
	char* buffer;

	fptr = fopen(path, "r");
	if(fptr == NULL) {
		printf("Error opening File.\n");
		return;
	}
	while ((nread = getline(&buffer, &length, fptr)) != -1) {
		printf("%s", buffer);
	}
	free(buffer);
}


// Function creates a new node
Node* create_node(int node_id, Node* left_child, Node* right_child, int decision_id) {
	Node* ret = calloc(1, sizeof(Node));
	ret->node_id = node_id;
	ret->left_child = left_child;
	ret->right_child = right_child;
	ret->decision_id = decision_id;
	return ret;
}
 
// Function converts input file into an array of strings
char** file_to_array(char* path, int num_vars, int num_nodes) {
	char** input_data_arr = calloc(num_vars, sizeof(char*));
	FILE* fptr;
	size_t length = 0; // Buffer's length in bytes
	ssize_t nread;// Variable stores the number of chars read into line
	char* buffer = NULL;

	fptr = fopen(path, "r");
	if(fptr == NULL) {
		printf("Error opening File.\n");
		return NULL;
	}
	// Throw out the first two lines (don't need this data)
	nread = getline(&buffer, &length, fptr);
	nread = getline(&buffer, &length, fptr);
	
	// Loop through each line, storing it's data
	for (int i = 0; i < num_vars; i++) {
		nread = getline(&buffer, &length, fptr);
		input_data_arr[i] = calloc(nread+1, sizeof(char));
		strcpy(input_data_arr[i], buffer);
	}
	fclose(fptr);
	free(buffer);
	return input_data_arr;
}

// Function returns the number of vars 
int get_num_vars(char* path) {
	FILE* fptr;
	size_t length = 0; // Buffer's length in bytes
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

// Function returns the number of nodes
int get_num_nodes(char* path) {
	FILE* fptr;
	size_t length = 0;
	char* buffer = NULL; // Buffer's length in bytes
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

// Funtions frees the input_data_arr
void free_input_data_arr(char** input_data_arr, int num_vars) {
	for(int i = 0; i < num_vars; i++) {
		free(input_data_arr[i]);
	}
	free(input_data_arr);
}
