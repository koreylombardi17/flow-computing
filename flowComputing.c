#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct B_Node B_Node;

struct B_Node {
	int id, decision_id;
	B_Node* left_child, *right_child;
};

void print_ints_arr(int**, int, int);
B_Node* create_bnode(int, B_Node*, B_Node*, int);
char** file_to_strings_arr(char*, int, int);
int** strings_arr_to_ints_arr(char**, int, int);
int get_num_vars(char*);
int get_num_bnodes(char*);
int get_num_rows(char*);
int get_num_cols(char*);
B_Node** create_bdd(char*, int);
int** create_xbar(char*, int, int);
void print_bdd(B_Node**, int);
void print_xbar(int**, int, int);
void write_xbar_to_file(int**, int, int);
void free_bdd(B_Node**, int);
void free_strings_arr(char**, int);
void free_ints_arr(int**, int);

int main() {
	//char* path = "input_files/bdds/var5.bdd";
	//int num_bnodes = get_num_bnodes(path);
	//B_Node** bdd = create_bdd(path, num_bnodes); // Task 1.1
	//print_bdd(bdd, num_bnodes); // Task 1.2
	// Free memory	
	//free_bdd(bdd, num_bnodes);
	
	char* path = "input_files/xbars/var5.xbar";
	int num_rows = get_num_rows(path);
	int num_cols = get_num_cols(path);
	int** xbar = create_xbar(path, num_rows, num_cols);
	print_xbar(xbar, num_rows, num_cols);
	write_xbar_to_file(xbar, num_rows, num_cols);
	free_ints_arr(xbar, num_rows);
	return 0;
}

// Function prints 2d int arr
void print_ints_arr(int** ints_arr, int num_rows, int num_cols) {	
	for(int i = 0; i < num_rows; i++) {
		for(int j = 0; j < num_cols; j++){
			printf("%d ", ints_arr[i][j]);
		}
		printf("\n");
	}
}

// Function creates a new node
B_Node* create_bnode(int id, B_Node* left_child, B_Node* right_child, int decision_id) {
	B_Node* ret = calloc(1, sizeof(B_Node));
	ret->id = id;
	ret->left_child = left_child;
	ret->right_child = right_child;
	ret->decision_id = decision_id;
	return ret;
}
 
// Function converts bdd input file into an array of strings
char** file_to_strings_arr(char* path, int num_nodes, int num_lines_skip) {
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
	// Don't need this data
	for(int i = 0; i < num_lines_skip; i++) {
		nread = getline(&buffer, &length, fptr);
	}

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
int** strings_arr_to_ints_arr(char** data_strings_arr, int num_rows, int num_cols) {
	int** ints_arr = calloc(num_rows, sizeof(int*));
	for(int i = 0; i < num_rows; i++) {
		ints_arr[i] = calloc(num_cols, sizeof(int));
	}
	
	char temp[3]; // Temp string that will store each value 
	memset(temp, '\0', 3); // Initialize temp string
	int temp_index = 0; // Index used for temp string
	int int_index = 0; // Index to keep track of which int is being worked on 
	// Loop through each variable
	for(int i = 0; i < num_rows; i++) {
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

// Function returns the number of vars for either type of file
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

// Function returns the number of bnodes from bdd file
int get_num_bnodes(char* path) {
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

// Function returns the num of rows from crossbar file
int get_num_rows(char* path) {
	FILE* fptr;
	size_t length = 0; // Buffer's length in bytes
	char* buffer = NULL;
	ssize_t nread;
	char num_rows_str[4]; // Max num rows being read is triple digits

	fptr = fopen(path, "r");
	if(fptr == NULL) {
		printf("Error opening file.\n");
	}

	// Copying input from second line, starting at the 5th element
	nread = getline(&buffer, &length, fptr);
	nread = getline(&buffer, &length, fptr);
	buffer+=5; // Position buffer's ptr to where the index of the num of rows exists
	
	int counter = 0;
	while(*buffer != '\n') {
		num_rows_str[counter++] = *buffer++;
	}
	num_rows_str[counter] = '\0';
	fclose(fptr);
	// Position buffer's ptr back to its original location to free memory correctly
	buffer-=(counter+5);
	free(buffer);
	return atoi(num_rows_str);
}

// Function return the num of columns from crossbar file
int get_num_cols(char* path) {
	FILE* fptr;
	size_t length = 0; // Buffer's length in bytes
	char* buffer = NULL;
	ssize_t nread;
	char num_cols_str[4]; // Max cols being read is triple digits

	fptr = fopen(path, "r");
	if(fptr == NULL) {
		printf("Error opening file.\n");
	}

	// Copying input from third line, starting at the 5th element
	nread = getline(&buffer, &length, fptr);
	nread = getline(&buffer, &length, fptr);
	nread = getline(&buffer, &length, fptr);
	buffer+=5; // Position buffer's ptr to where the index of the num of cols exists
	
	int counter = 0;
	while(*buffer != '\n') {
		num_cols_str[counter++] = *buffer++;
	}
	num_cols_str[counter] = '\0';
	fclose(fptr);
	// Position buffer's ptr back to its original location to free memory correctly
	buffer-=(counter+5);
	free(buffer);
	return atoi(num_cols_str);
}

// Function creates and returns the bdd
B_Node** create_bdd(char* path, int num_nodes) {
	int num_vars = get_num_vars(path);
	int left_child_index = 0;
	int right_child_index = 0;
	// First parse File to arr of strings, then convert to 2d arr of ints
	char** data_strings_arr = file_to_strings_arr(path, num_nodes, 2);// Skip first 2 lines of in data
	int** data_ints_arr = strings_arr_to_ints_arr(data_strings_arr, num_nodes, num_vars);

	// Allocate memory
	B_Node** bdd = calloc(num_nodes, sizeof(B_Node*));
	for(int i = 0; i < num_nodes; i++) {
		bdd[i] = create_bnode(data_ints_arr[i][0], NULL, NULL, data_ints_arr[i][3]);
	}

	// Connect left and right children nodes. If children nodes don't exists, they remain NULL
	for(int i = 0; i < num_nodes; i++) {
		left_child_index = data_ints_arr[i][1] - 1;
		right_child_index = data_ints_arr[i][2] - 1;
		if(!(left_child_index < 0)){
			bdd[i]->left_child = bdd[left_child_index];
			bdd[i]->right_child = bdd[right_child_index];
		}
	}

	free_strings_arr(data_strings_arr, num_nodes);	
	free_ints_arr(data_ints_arr, num_nodes);
	return bdd;
}

// Function creates and returns the crossbar
int** create_xbar(char* path, int num_rows, int num_cols) {
	// First parse File to arr of strings, then convert to 2d arr of ints
	char** data_strings_arr = file_to_strings_arr(path, num_rows, 3); // Skip first 3 lines of in data 
	int** xbar = strings_arr_to_ints_arr(data_strings_arr, num_rows, num_cols);
	free_strings_arr(data_strings_arr, num_rows);
	return xbar;
}

// Function prints the bdd in the same format it was recieved
void print_bdd(B_Node** bdd, int num_nodes) {
	// Loop through each line
	// Line format: node_id, left_child_id, right_child_id, decision_id
	for(int i = 0; i < num_nodes; i++) {
		// Print node_id
		printf("%d ", bdd[i]->id);
		// Print left_child_id. If its null, print -1
		if(bdd[i]->left_child != NULL){
			printf("%d ", bdd[i]->left_child->id);
		} else {
			printf("-1 ");
		}
		// Print right_child_id. If its null, print -1
		if(bdd[i]->right_child != NULL){
			printf("%d ", bdd[i]->right_child->id);
		} else {
			printf("-1 ");
		}
		// Print decision_id
		printf("%d\n", bdd[i]->decision_id);
	}
}

// Function prints the xbar
void print_xbar(int** xbar, int num_rows, int num_cols) {
	print_ints_arr(xbar, num_rows, num_cols);
}

//Function writes the xbar to a file
void write_xbar_to_file(int** xbar, int num_rows, int num_cols) {
	FILE* fptr;
	fptr = fopen("xbar_output.txt", "w");

	if(fptr == NULL) {
		printf("Error writing to file.\n");
	}
	// Loop through 2d int arr printing content to file
	for(int i = 0; i < num_rows; i++) {
		for(int j = 0; j < num_cols; j++){
			fprintf(fptr, "%d ", xbar[i][j]);
		}
		fprintf(fptr, "\n");
	}
	fclose(fptr);
}

// Function frees the bdd
void free_bdd(B_Node** bdd, int num_nodes) {
	for(int i = 0; i < num_nodes; i++) {
		free(bdd[i]);
	}
	free(bdd);
}

// Funtion frees the input_data_arr
void free_strings_arr(char** input_data_arr, int num_nodes) {
	for(int i = 0; i < num_nodes; i++) {
		free(input_data_arr[i]);
	}
	free(input_data_arr);
}

// Funtion frees the input_ints_arr
void free_ints_arr(int** ints_arr, int num_nodes) {
	for(int i = 0; i < num_nodes; i++) {
		free(ints_arr[i]);
	}
	free(ints_arr);
}
