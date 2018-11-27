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

void worst_fit_create_node(node_t* node, node_t* new_node){
	head_t* head = worst_fit_mem_start;
	new_node -> next_node = node -> next_node;
	new_node -> prev_node = node;
	new_node -> filled = 0;
	if(node -> next_node == NULL)
		new_node -> mem_size = ((intptr_t)head -> end_of_mem) - ((intptr_t)new_node + sizeof(node_t));
	else
		new_node -> mem_size = (new_node -> next_node) - ((intptr_t)new_node + sizeof(node_t));		
		
	int offset_four_byte = ((intptr_t)new_node + sizeof(node_t)) % 4;		
	new_node -> mem_start = (intptr_t)new_node + sizeof(node_t) + offset_four_byte;
	new_node -> mem_size -= offset_four_byte;
	node -> next_node = new_node;	
}

/* Functions */

/* memory initializer */
int best_fit_memory_init(size_t size)
{
	best_fit_mem_start = malloc(size);
	return 0;

}

int worst_fit_memory_init(size_t size)
{
	if(size < (sizeof(node_t) + sizeof(head_t))){
		printf("Error: Size too small");
		return 0;
	}
		
	
	worst_fit_mem_start = malloc(size);
	printf("Start of memory is located at %x \n",worst_fit_mem_start);
	head_t* head = worst_fit_mem_start;
	head->first_node = sizeof(head_t) + worst_fit_mem_start;
	head->end_of_mem = worst_fit_mem_start + size;
	printf("End of memory is located at %x \n",head->end_of_mem);
	node_t* first_node = head->first_node;
	first_node -> next_node = NULL;
	first_node -> prev_node = NULL;
	first_node -> mem_start = first_node + sizeof(node_t);
	first_node -> mem_size = size - (sizeof(head_t) + sizeof(node_t));
	first_node -> filled = 0;

	printf("First memory location available at %x with space %d \n",first_node->mem_start,first_node->mem_size);
	printf("Size of head %d and size of node %d",sizeof(head_t),sizeof(node_t));
	return 0;

}

/* memory allocators */
void *best_fit_alloc(size_t size)
{
	
	return NULL;
}


void *worst_fit_alloc(size_t size)
{
	node_t* new_node;
	node_t* largest_available = NULL;
	size_t largest_size = -1;

	head_t* head = worst_fit_mem_start;
	node_t* itr = head->first_node;

	// Check for first entry aka base case
	if(itr->next_node == NULL){
		if(size > (itr->mem_size + sizeof(node_t))){
			//error
			return 1;
		}
		itr->next_node = itr + size + sizeof(node_t);
		itr->mem_start = itr + sizeof(node_t);
		itr->mem_size = size;
		itr->filled = 1;

		new_node = itr->next_node;
		new_node->next_node = NULL;
		new_node->prev_node = itr;
		new_node->mem_start = new_node + sizeof(node_t);
		new_node->mem_size = head->end_of_mem - new_node->mem_start;
		new_node->filled = 0;
	}

	// Loop to search for largest available memory location
	while(itr != NULL){
		if(itr->filled == 0 && itr->mem_size > largest_size){
			largest_available = itr;
			largest_size = itr->mem_size;
		}
	}

	// If available space of sufficient size was found
	if(largest_size > size + sizeof(node_t)){
		itr = largest_available; //Focus itr onto largest available node
		node_t* new_node = (intptr_t)itr + sizeof(node_t) + size;
		worst_fit_create_node(itr,new_node);
		return itr->mem_start;
	}

	return NULL;
}

/* memory de-allocator */
void best_fit_dealloc(void *ptr) 
{

	// To be completed by students
	return;
}

void worst_fit_dealloc(void *ptr) 
{

	// To be completed by students
	return;
}

/* memory algorithm metric utility function(s) */

/* count how many free blocks are less than the input size */ 
int best_fit_count_extfrag(size_t size)
{
	// To be completed by students
	return 0;
}

int worst_fit_count_extfrag(size_t size)
{
	// To be completed by students
	return 0;
}
