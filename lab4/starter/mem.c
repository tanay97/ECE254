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

/* defines */

/* global variables */

void * best_fit_start_ptr;
void * worst_fit_start_ptr; 

/* Functions */

/*typedef struct linked_list { 

	size_t total_alloc;	
	struct node * head;

}mem_list; //size 16 bytes
*/

typedef struct node {
	size_t alloc_size;
	int free;
	struct node * next;
	struct node * prev;
}node_t; //size 32 bytes

node_t * bfsp_head;
node_t * wfsp_head;

/* memory initializer */
int best_fit_memory_init(size_t size)
{
	
	// To be completed by students
	// You call malloc once here to obtain the memory to be managed.
	
	bfsp_head = (node_t *) malloc(size);
	printf("Start of the memblock is: %p and its size is %d bytes. \n", bfsp_head, sizeof(node_t));
	bfsp_head->alloc_size = size - sizeof(node_t);
	//node_t * head;
	bfsp_head->free = 1;
	//printf("total alloc is: %d. size of mem list %d bytes. %d bytes left.\n", bfsp_mem_list->total_alloc, sizeof(mem_list), bfsp_mem_list->head->alloc_size) ;
	//printf("node head is located at: %p and its size is %d \n", bfsp_mem_list->head, sizeof(node_t));

	/*node_t * ptr = bfsp_head; 
	printf("ptr + 1 = %p \n", (node_t*)((int) ptr + 1));*/	
	return 0;

}

int worst_fit_memory_init(size_t size)
{

	// To be completed by students
	// You call malloc once here to obtain the memory to be managed.
	return 0;

}

/* memory allocators */
void *best_fit_alloc(size_t size)
{
	// To be completed by students
		
	if(bfsp_head->next == NULL){
		bfsp_head->next = (node_t *)((void*)bfsp_head + sizeof(node_t) + size);
		printf("adding %p and %d giving %p in hex \n", bfsp_head, sizeof(node_t) + size, bfsp_head->next);
		bfsp_head->next->alloc_size = bfsp_head->alloc_size - sizeof(node_t) - size; 
		bfsp_head->alloc_size = size;
		bfsp_head->free = 0;
		 
		printf("new node at %p with size %d \n", bfsp_head->next, bfsp_head->next->alloc_size);
		return (node_t*) ((void*)bfsp_head + sizeof(node_t));

	}

	else{
	
		for(node_t * runner = bfsp_head; runner != NULL; runner = runner->next){
	 			
		}	
	}
	

	return NULL;
}


void *worst_fit_alloc(size_t size)
{
	// To be completed by students
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
