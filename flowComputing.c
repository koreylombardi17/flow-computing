#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node Node;

struct Node {
	int node_id, decision_id;
	Node* left_child, *right_child;
};

void print_input_file(char*); // TODO: Maybe delete this function later
void print_strings_arr(char**, int); // TODO: Delete later
void print_ints_arr(int**, int); // TODO: Delete later
Node* create_node(int, Node*, Node*, int);
char** file_to_strings_arr(char*, int, int);
int** strings_arr_to_ints_arr(char**, int, int);
int get_num_vars(char*);
int get_num_nodes(char*);
Node* create_bdd(int**, int);
void free_strings_arr(char**, int);
void free_ints_arr(int**, int);

int main() {
	char* path = "input_files/bdds/var2.bdd";
	int num_vars = get_num_vars(path);
	int num_nodes = get_num_nodes(path);
	char** data_strings_arr = file_to_strings_arr(path, num_vars, num_nodes);
	int** data_ints_arr = strings_arr_to_ints_arr(data_strings_arr, num_vars, num_nodes);
	
	Node* head = create_bdd(data_ints_arr, num_nodes);

	// Free memory
	free_strings_arr(data_strings_arr, num_nodes);	
	free_ints_arr(data_ints_arr, num_nodes);
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

void print_strings_arr(char** strings_arr, int num_nodes) {
	printf("Printing as string array.\n");	
	for(int i = 0; i < num_nodes; i++) {
		printf("%s", strings_arr[i]);
	}
}

void print_ints_arr(int** ints_arr, int num_nodes) {
	printf("Printing as int array.\n");	
	for(int i = 0; i < num_nodes; i++) {
		for(int j = 0; j < 4; j++){
			printf("%d ", ints_arr[i][j]);
		}
		printf("\n");
	}
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
char** file_to_strings_arr(char* path, int num_vars, int num_nodes) {
	char** input_data_arr = calloc(num_nodes, sizeof(char*));
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
	for (int i = 0; i < num_nodes; i++) {
		nread = getline(&buffer, &length, fptr);
		input_data_arr[i] = calloc(nread+1, sizeof(char));
		strcpy(input_data_arr[i], buffer);
	}
	fclose(fptr);
	free(buffer);
	return input_data_arr;
}

// Function converts the arr of strings into a 2d array of ints
int** strings_arr_to_ints_arr(char** data_strings_arr, int num_vars, int num_nodes) {
	int** ints_arr = calloc(num_nodes, sizeof(int*));
	for(int i = 0; i < num_nodes; i++) {
		ints_arr[i] = calloc(4, sizeof(int)); // 4 int's per line of data
	}
	
	char temp[3]; // Temp string that will store each value 
	memset(temp, '\0', 3); // Initialize temp string
	int temp_index = 0; // Index used for temp string
	int int_index = 0; // Index to keep track of which int is being worked on 
	// Loop through each variable
	for(int i = 0; i < num_nodes; i++) {
		char* original_ptr_location = data_strings_arr[i]; // Used to psoition pointer to its original location
		// Loop to the end of line
		while(*data_strings_arr[i] != '\n') { 
			// Loop until a space is reached
			while((*data_strings_arr[i] != ' ') && (*data_strings_arr[i] != '\n')) { 
				temp[temp_index++] = *data_strings_arr[i]++;
			}
			ints_arr[i][int_index++] = atoi(temp);
			temp_index = 0; // Reset temp index in order to reset temp string
			memset(temp, '\0', 3); // Reinitialize temp string
			if(*data_strings_arr[i] != '\n') {
				data_strings_arr[i]++; // Move from ' ' to next char
			}
		}
		data_strings_arr[i] = original_ptr_location;
		int_index = 0; // Reset int index since moving to the next line
	}
	return ints_arr;
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
	buffer+=5; // Position buffer's ptr to where the index of the num of vars exists
	
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
	buffer+=6; // Position buffer's ptr to where the index of the num of nodes exists
	
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

Node* create_bdd(int** ints_arr, int num_nodes) {
	Node* nodes_arr[num_nodes];
	int left_child_index = 0;
	int right_child_index = 0;
	
	for(int i = 0; i < num_nodes; i++) {
		nodes_arr[i] = create_node(ints_arr[i][0], NULL, NULL, ints_arr[i][3]);
	}

	for(int i = 0; i < num_nodes; i++) {
		left_child_index = ints_arr[i][1] - 1;
		right_child_index = ints_arr[i][2] - 1;
		if(!(left_child_index < 0)){
			nodes_arr[i]->left_child = nodes_arr[left_child_index];
			nodes_arr[i]->right_child = nodes_arr[right_child_index];
		}
	}
	return nodes_arr[0];
}

// Funtion frees the input_data_arr
void free_strings_arr(char** input_data_arr, int num_vars) {
	for(int i = 0; i < num_vars; i++) {
		free(input_data_arr[i]);
	}
	free(input_data_arr);
}

// Funtion frees the input_ints_arr
void free_ints_arr(int** ints_arr, int num_vars) {
	for(int i = 0; i < num_vars; i++) {
		free(ints_arr[i]);
	}
	free(ints_arr);
}
