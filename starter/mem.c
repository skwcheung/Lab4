/**
 * @file memory.c
 * @brief: ECE254 Lab: memory allocation algorithm comparison template file
 * @author: 
 * @date: 2015-11-20
 */

/* includes */
#include <stdio.h> 
#include <stdlib.h> 
#include "mem.h"
#include<stdint.h>

/* defines */

/* global variables */
void* best_fit_mem_start;
void* worst_fit_mem_start;

typedef struct node{
	struct node* next_node;
	struct node* prev_node;
	void* mem_start;
	size_t mem_size;
	unsigned int filled;
}node_t;

typedef struct head{
	struct node* first_node;
	void* end_of_mem;
}head_t;

// The offset is already accounted for when passing in new_node
void worst_fit_create_node(node_t* node, node_t* new_node,size_t size){
	head_t* head = worst_fit_mem_start;
	node->filled = 1;
	new_node -> next_node = node -> next_node;
///	printf("current node is %d and next node is %d \n",node,new_node);
	new_node -> prev_node = node;
	new_node -> filled = 0;
	if(node -> next_node == NULL){
		new_node -> mem_size = ((intptr_t)head -> end_of_mem) - ((intptr_t)new_node + sizeof(node_t));
	}
	else{
		new_node ->next_node->prev_node = new_node;
//		printf("Current node is %d and next node is %d \n",new_node,new_node->next_node);
		// new_node -> mem_size = (new_node -> next_node) - ((intptr_t)new_node + sizeof(node_t));	
		new_node -> mem_size = (node->mem_size) - (size + sizeof(node_t));		
	}	

	new_node -> mem_start = (intptr_t)new_node + sizeof(node_t); // The start of new node is just its start address plus node
	node -> next_node = new_node;	
	node->mem_size = ((intptr_t)new_node - (intptr_t)node->mem_start);
}

void best_fit_create_node(node_t* node, node_t* new_node,size_t size){
	head_t* head = best_fit_mem_start;
	node->filled = 1;
	new_node -> next_node = node -> next_node;
//	printf("current node is %d and next node is %d \n",node,new_node);
	new_node -> prev_node = node;
	new_node -> filled = 0;
	if(node -> next_node == NULL){
		new_node -> mem_size = ((intptr_t)head -> end_of_mem) - ((intptr_t)new_node + sizeof(node_t));
	}
	else{
		new_node ->next_node->prev_node = new_node;
//		printf("Current node is %d and next node is %d \n",new_node,new_node->next_node);
		new_node -> mem_size = (new_node -> next_node) - ((intptr_t)new_node + sizeof(node_t));	
		new_node -> mem_size = (node->mem_size) - (size + sizeof(node_t));		
	}	

	new_node -> mem_start = (intptr_t)new_node + sizeof(node_t); // The start of new node is just its start address plus node
	node -> next_node = new_node;	
	node->mem_size = ((intptr_t)new_node - (intptr_t)node->mem_start);
}

void combine_nodes(node_t* node, node_t* second_node){
	node->next_node = second_node->next_node;
//	node->next_node->prev_node = second_node;
	node->mem_size = (intptr_t)(node->mem_size) + sizeof(node_t) + second_node->mem_size;
}

void print_mem(int mode){ // 1 is worst 0 is best
	head_t* head;
	if(mode)
		head = worst_fit_mem_start;
	else	
		head = best_fit_mem_start;
	node_t* itr = head->first_node;
	printf("...PRINTING MEMORY... \n");
	while(itr != NULL){
//		printf("Current mem_start is %d is filled: %d and mem_size: %d next_node is: %d \n", itr->mem_start,itr->filled,itr->mem_size,itr->next_node);
		itr = itr->next_node;
	}
}

/* Functions */

/* memory initializer */
int best_fit_memory_init(size_t size)
{
	//printf(" \n**************** START BEST INIT **************\n \n");
	if(size < (sizeof(node_t) + sizeof(head_t))){
		printf("ERROR: Size too small");
		return -1;
	}
	
	best_fit_mem_start = malloc(size);
	//printf("Start of memory is located at %d \n",best_fit_mem_start);
	head_t* head = best_fit_mem_start;
	head->first_node = sizeof(head_t) + best_fit_mem_start;
	head->end_of_mem = best_fit_mem_start + size;
	//printf("End of memory is located at %d \n",head->end_of_mem);
	node_t* first_node = head->first_node;
	first_node -> next_node = NULL;
	first_node -> prev_node = NULL;
	first_node -> mem_start = first_node + sizeof(node_t);
	first_node -> mem_size = size - (sizeof(head_t) + sizeof(node_t));
	first_node -> filled = 0;

	//printf(" \nFirst memory location available at %x with space %d\n",first_node->mem_start,first_node->mem_size);
	//printf("Size of head %d and size of node %d \n",sizeof(head_t),sizeof(node_t));
	//printf(" \n**************** FINISH INIT **************\n \n");
	return 0;
}

int worst_fit_memory_init(size_t size)
{
	//printf(" \n**************** START WORST INIT **************\n \n");
	if(size < (sizeof(node_t) + sizeof(head_t))){
		printf("ERROR: Size too small");
		return -1;
	}
	
	worst_fit_mem_start = malloc(size);
	//printf("Start of memory is located at %d \n",worst_fit_mem_start);
	head_t* head = worst_fit_mem_start;
	head->first_node = sizeof(head_t) + worst_fit_mem_start;
	head->end_of_mem = worst_fit_mem_start + size;
	//printf("End of memory is located at %d \n",head->end_of_mem);
	node_t* first_node = head->first_node;
	first_node -> next_node = NULL;
	first_node -> prev_node = NULL;
	first_node -> mem_start = first_node + sizeof(node_t);
	first_node -> mem_size = size - (sizeof(head_t) + sizeof(node_t));
	first_node -> filled = 0;

	//printf(" \nFirst memory location available at %x with space %d\n",first_node->mem_start,first_node->mem_size);
	//printf("Size of head %d and size of node %d \n",sizeof(head_t),sizeof(node_t));
	//printf(" \n**************** FINISH INIT **************\n \n");
	return 0;

}

/* memory allocators */
void *best_fit_alloc(size_t size)
{
	//printf("===================== CALL OF BEST ALLOC =================== \n");
	node_t* new_node;
	node_t* smallest_available = NULL;
	size_t smallest_size = 0;

	head_t* head = best_fit_mem_start;
	node_t* itr = head->first_node;

	// Check for first entry aka base case
	if(itr->next_node == NULL){
		if(size > (itr->mem_size + sizeof(node_t))){
			//ERROR
			printf("ERROR: Size is too large");
			return 1;
		}
		itr->next_node = (intptr_t)itr + size + sizeof(node_t);
		int offset_four_byte = ((intptr_t)itr->next_node) % 4;
		itr->next_node = (intptr_t)(itr->next_node) + offset_four_byte;
		itr->mem_start = (intptr_t)itr + sizeof(node_t);
		itr->mem_size = size + offset_four_byte;
		itr->filled = 1;
		if(((intptr_t)(head->end_of_mem) - (intptr_t)(itr->next_node) < sizeof(node_t))){
			printf("ERROR: Not enough space to create second node \n");
			return itr;
		}
		new_node = itr->next_node;
		new_node->next_node = NULL;
		new_node->prev_node = itr;
		new_node->mem_start = (intptr_t)new_node + sizeof(node_t);
		new_node->mem_size = head->end_of_mem - new_node->mem_start;
		new_node->filled = 0;

	//	printf("Created first node %d with size %d and with the next node occuring at %d \n",(unsigned long)itr,size,(unsigned long)(itr->next_node));
	//	printf("===================== FINISH ALLOC =================== \n");
		return itr;
	}
	// Loop to search for smallest available memory locations
	while(itr != NULL && itr < head->end_of_mem){
		if(itr->filled == 0 ){
			if(smallest_available == NULL){ //EDGE case to setup the first smallest available
				smallest_available = itr;
				smallest_size = itr->mem_size;
			}
			if(itr->mem_size < smallest_size){
				smallest_available = itr;
				smallest_size = itr->mem_size;
			}
			
		}
		itr = itr->next_node;
	}

	// If available space of sufficient size was found
	if(smallest_size > (size + sizeof(node_t))){
		itr = smallest_available; //Focus itr onto largest available node
		node_t* new_node = (intptr_t)itr + sizeof(node_t) + size;
		int offset_four_byte = 4 - (size % 4); // New node has already been previous node + size + size of node (so now see if we need to add any offset so next will start byte aligned)
		if(offset_four_byte == 4){
			offset_four_byte = 0;
		}
	//	printf("Creating new node with size %d \n",size + offset_four_byte);
		new_node = (intptr_t)new_node + offset_four_byte; 
		best_fit_create_node(itr,new_node,size+offset_four_byte);
	//	printf("===================== FINISH ALLOC =================== \n");
		return itr->mem_start;
	}
	printf("ERROR: Unable to find space for request of size %d \n",size);
	return NULL;
}


void *worst_fit_alloc(size_t size)
{
	//printf("===================== CALL OF WORST ALLOC =================== \n");
	node_t* new_node;
	node_t* largest_available = 0;
	size_t largest_size = 0;

	head_t* head = worst_fit_mem_start;
	node_t* itr = head->first_node;

	// Check for first entry aka base case
	if(itr->next_node == NULL){
		if(size > (itr->mem_size + sizeof(node_t))){
			//ERROR
			printf("ERROR: Size is too large");
			return 1;
		}
		itr->next_node = (intptr_t)itr + size + sizeof(node_t);
		int offset_four_byte = ((intptr_t)itr->next_node) % 4;
		itr->next_node = (intptr_t)(itr->next_node) + offset_four_byte;
		itr->mem_start = (intptr_t)itr + sizeof(node_t);
		itr->mem_size = size + offset_four_byte;
		itr->filled = 1;
		if(((intptr_t)(head->end_of_mem) - (intptr_t)(itr->next_node) < sizeof(node_t))){
	//		printf("ERROR: Not enough space to create second node \n");
			return itr;
		}
		new_node = itr->next_node;
		new_node->next_node = NULL;
		new_node->prev_node = itr;
		new_node->mem_start = (intptr_t)new_node + sizeof(node_t);
		new_node->mem_size = head->end_of_mem - new_node->mem_start;
		new_node->filled = 0;

	//	printf("Created first node %d with size %d and with the next node occuring at %d \n",(unsigned long)itr,size,(unsigned long)(itr->next_node));
	//	printf("===================== FINISH ALLOC =================== \n");
		return itr;
	}
	// Loop to search for largest available memory location
	while(itr != NULL && itr < head->end_of_mem){
		if((itr->filled == 0) && (itr->mem_size > largest_size)){
			largest_available = itr;
			largest_size = itr->mem_size;
		}
		itr = itr->next_node;
	}

	// If available space of sufficient size was found
	if(largest_size > size + sizeof(node_t)){
		itr = largest_available; //Focus itr onto largest available node
		node_t* new_node = (intptr_t)itr + sizeof(node_t) + size;
	//	printf("Creating new node with size %d \n",size);
		int offset_four_byte = 4 - (size % 4); // New node has already been previous node + size + size of node (so now see if we need to add any offset so next will start byte aligned)
		if(offset_four_byte == 4){
			offset_four_byte = 0;
		}
		new_node = (intptr_t)new_node + offset_four_byte; 
		worst_fit_create_node(itr,new_node,size+offset_four_byte);
	//	printf("===================== FINISH ALLOC =================== \n");
		return itr->mem_start;
	}
	//printf("ERROR: Unable to find space for request of size %d \n",size);
	return NULL;
}

/* memory de-allocator */
void best_fit_dealloc(void *ptr) 
{
	//printf("===================== START DEALLOC =================== \n");
	head_t* head = best_fit_mem_start;
	node_t* itr = head->first_node;
	if((ptr < head) || (ptr > head->end_of_mem)){
		printf("ERROR: Requested deallocation out of memory range, %x, %x, %x \n", ptr, head, head->end_of_mem);
		return;
	}

	while(itr != NULL){
		if(itr->mem_start == ptr){
			//printf("SUCCESS: Deallocated ptr %d \n",ptr);
			if (itr == NULL) printf("badd\n"); 
			//printf("val: %d\n", itr->filled); 
			itr->filled = 0;
			//printf("before if statement");
			if((itr->filled == 0) && (itr->next_node->filled == 0)){ // Checking for consecutive unfilled memory spaces to join
			//	printf("before combine");
				combine_nodes(itr,itr->next_node);
			//	printf("SUCCESS: Combined two empty adjacent spaces \n");
			}
			//printf("===================== FINISH DEALLOC =================== \n");
			return;
		}
		itr = itr->next_node;
	}
	//printf("===================== FINISH DEALLOC =================== \n");	
	return;
}

void worst_fit_dealloc(void *ptr) 
{
	//printf("===================== START DEALLOC =================== \n");
	head_t* head = worst_fit_mem_start;
	node_t* itr = head->first_node;
	if((ptr < head) || (ptr > head->end_of_mem)){
		printf("ERROR: Requested deallocation out of memory range \n");
		return;
	}

	while(itr != NULL){
		if(itr->mem_start == ptr){
	//		printf("SUCCESS: Deallocated ptr %d \n",ptr);
			itr->filled = 0;
			if((itr->filled == 0) && (itr->next_node->filled == 0)){ // Checking for consecutive unfilled memory spaces to join
				combine_nodes(itr,itr->next_node);
	//			printf("SUCCESS: Combined two empty adjacent spaces \n");
			}
	//		printf("===================== FINISH DEALLOC =================== \n");
			return;
		}
		itr = itr->next_node;
	}
	//printf("===================== FINISH DEALLOC =================== \n");	
	return;
}

/* memory algorithm metric utility function(s) */

/* count how many free blocks are less than the input size */ 
int best_fit_count_extfrag(size_t size)
{
	head_t* head = best_fit_mem_start;
	node_t* itr = head->first_node;
	int counter = 0;
	while(itr != NULL){
		if((itr->mem_size < size) && (itr->filled == 0)){
			counter++;
		}
		itr = itr->next_node;
	}
	return counter;
}

int worst_fit_count_extfrag(size_t size)
{
	head_t* head = worst_fit_mem_start;
	node_t* itr = head->first_node;
	int counter = 0;
	while(itr != NULL){
		if((itr->mem_size < size) && (itr->filled == 0)){
			counter++;
		}
		itr = itr->next_node;
	}
	return counter;
}
