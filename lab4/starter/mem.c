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
		
	int adjustment = 0;	
	if(bfsp_head->next == NULL){ 
		
		void * ptr = (void*)bfsp_head + sizeof(node_t) + size;
		if((int) ptr %4 != 0){
			adjustment = 4 - (int)ptr%4;
			ptr += 4 - (int)ptr%4;
		}
			
		bfsp_head->next = (node_t *)(ptr);
	
		printf("adding %p and %d giving %p in hex \n", bfsp_head, sizeof(node_t) + size, bfsp_head->next);
		bfsp_head->next->alloc_size = bfsp_head->alloc_size - sizeof(node_t) - size - adjustment; 
		bfsp_head->next->free = 1;
		bfsp_head->alloc_size = size + adjustment;
		bfsp_head->free = 0;
		
		printf("node at %p with size %d \n", bfsp_head, bfsp_head->alloc_size);
		return (void*) ((void*)bfsp_head + sizeof(node_t));

	}

	else{
		int min = 100000;
		node_t * bf_node = NULL;	
		for(node_t * runner = bfsp_head; runner != NULL; runner = runner->next){
			if(runner->alloc_size >= size + sizeof(node_t) && runner->alloc_size < min && runner->free == 1 ){
				min = runner->alloc_size;
				bf_node = runner;
			}
		}	
		if (bf_node) {
			if(bf_node->alloc_size - size - 4 > sizeof(node_t)){//if there's enough space for one more node and 4 more for 0mod4 address
				node_t * temp = bf_node->next;
				
				void * ptr = (void *)bf_node + size + sizeof(node_t);
				if ((int)ptr % 4 != 0){
					//printf("adding %d more to ptr %p \n", 4-(int)ptr%4, ptr);	
					adjustment = 4 - (int)ptr%4;
					ptr += 4 - (int)ptr%4;
				}	
				bf_node->next = (node_t *) (ptr); 

				bf_node->next->alloc_size = bf_node->alloc_size - sizeof(node_t) - size - adjustment; 
				bf_node->next->free = 1;
			
				bf_node->next->next = temp;
				bf_node->next->prev = bf_node;

				if (temp){
					temp->prev = bf_node->next;
				}
			}
			bf_node->free = 0;
			bf_node->alloc_size = size + adjustment;
		
			printf("node at %p with size %d \n", bf_node, bf_node->alloc_size);
			return (void *)bf_node + sizeof(node_t);	
		}
	}
	

	return NULL;
}


void *worst_fit_alloc(size_t size)
{
	int adjustment = 0;
	if (wfsp_head-> next == NULL) {
		// No memory
		void * ptr = (void*)wfsp_head + sizeof(node_t) + size;
		if((int) ptr %4 != 0){
			ptr += 4 - (int)ptr%4;
			adjustment = 4 - (int)ptr%4;
		}
			
		wfsp_head->next = (node_t *)(ptr);

		wfsp_head->next->alloc_size = wfsp_head->alloc_size - sizeof(node_t) - size - adjustment;
		wfsp_head->next->free = 1;
		wfsp_head->alloc_size = size + adjustment;
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
			
			void * ptr = (void *)maxNode + size + sizeof(node_t);
			if ((int)ptr % 4 != 0){
				//printf("adding %d more to ptr %p \n", 4-(int)ptr%4, ptr);
				ptr += 4 - (int)ptr%4;
				adjustment = 4 - (int)ptr%4;
			}	
			maxNode->next = (node_t *) (ptr); 

			maxNode->next->alloc_size = maxNode->alloc_size - sizeof(node_t) - size - adjustment;
			maxNode->next->free = 1;
			maxNode->alloc_size = size + adjustment;
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
	node_t* del_ptr = (node_t*) ((void*)ptr - sizeof(node_t));

	// If node after target is free join the memory together
	if (del_ptr->next->free == 1) {
		del_ptr->alloc_size = del_ptr->alloc_size + del_ptr->next->alloc_size + sizeof(node_t);
		del_ptr->next = del_ptr->next->next;
	}
		
	// If node before target is free join the memory together
	for (node_t* runner = bfsp_head; runner != NULL; runner = runner->next) {
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
	int count = 0;
	for(node_t* runner = wfsp_head; runner != NULL; runner = runner->next){
		if (runner->free && runner->alloc_size > size) {
			count++;
			printf("Node %p with size %d is free", runner, runner->alloc_size);
		}
	}	
	return count;
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
