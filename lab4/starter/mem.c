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
	if (size < 0 || size % 4 != 0) {
		return -1;
	}

	wfsp_head = (node_t *) malloc(size);
	printf("Start of memory at: %p, block size is %d bytes\n", wfsp_head, sizeof(node_t));
	wfsp_head->alloc_size = size - sizeof(node_t);

	wfsp_head->free = 1;

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
	if (wfsp_head-> next == NULL) {
		// No memory
		wfsp_head->next = (node_t *) ((void*)wfsp_head + sizeof(node_t) + size);

		wfsp_head->next->alloc_size = wfsp_head->alloc_size - sizeof(node_t) - size;
		wfsp_head->next->free = 1;
		wfsp_head->alloc_size = size;
		wfsp_head->free = 0;

		return (node_t*) ((void*)wfsp_head + sizeof(node_t));
	} else {
		int maxMem = 0;
		node_t* maxNode = wfsp_head;
		for (node_t* runner = wfsp_head; runner != NULL; runner = runner->next) {
			// Find the largest memory block available
			if (runner->free == 1 && runner->alloc_size > maxMem) {
				maxMem = runner->alloc_size;
				maxNode = runner;
			}
		}	

		if (maxMem > size) {
			maxNode->next = (node_t *) ((void*)maxNode + sizeof(node_t) + size);

			maxNode->next->alloc_size = maxNode->alloc_size - sizeof(node_t) - size;
			maxNode->next->free = 1;
			maxNode->alloc_size = size;
			maxNode->free = 0;

			return (node_t*) ((void*)maxNode + sizeof(node_t));
		} else if (maxMem == size) {

			maxNode->free = 0;

			return (node_t*) ((void*)maxNode + sizeof(node_t));
		} else {
			// No Space available
			return NULL;
		}
	}

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
	node_t* del_ptr = (node_t*) ((void*)ptr - sizeof(node_t));

	// If node after target is free join the memory together
	if (del_ptr->next->free == 1) {
		del_ptr->alloc_size = del_ptr->alloc_size + del_ptr->next->alloc_size + sizeof(node_t);
		del_ptr->next = del_ptr->next->next;
	}
		
	// If node before target is free join the memory together
	for (node_t* runner = wfsp_head; runner != NULL; runner = runner->next) {
		if (runner->next == del_ptr && runner->free == 1) {
			runner->alloc_size = runner->alloc_size + del_ptr->alloc_size + sizeof(node_t);
			runner->next = del_ptr->next;
			break;
		}
	}

	del_ptr->free = 1;
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
	int count = 0;
	for (node_t* runner = wfsp_head; runner != NULL; runner = runner->next) {
		printf("Node location: %p with size %d with free: %d\n", runner, runner->alloc_size, runner->free);		
		if (runner->free && runner->alloc_size > size) {
			count++;
		}
	}
	return count;
}
