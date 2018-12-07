/**
  * ECE254 Linux Dynamic Memory Management Lab
  * @file: main_test.c
  * @brief: The main file to write tests of memory allocation algorithms
  */ 

/* includes */
/* system provided header files. You may add more */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* non-system provided header files. 
   Do not include more user-defined header files here
 */
#include "mem.h"



int main(int argc, char *argv[])
{
	int num = 0;
	int algo = 0; // default algorithm to test is best fit  
	void *ptrs[10];

	if (argc != 2) {
		fprintf(stderr, "Usage: %s <0/1>. 0 for best fit and 1 for worst fit \n", argv[0]);
		exit (1);
	} else if (!strcmp(argv[1], "1") || !strcmp(argv[1], "0")) {
		algo = atoi(argv[1]);
	} else {
		fprintf(stderr, "Invalid argument, please specify 0 or 1\n");
		exit(1);
	}
	
	if ( algo == 0 ) {
		// Best fit test cases

		best_fit_memory_init(1024);	// initizae 1KB, best fit

		ptrs[0] = best_fit_alloc(8);		// allocate 8B
		ptrs[1] = best_fit_alloc(16);		// allocate 16B
		ptrs[2] = best_fit_alloc(14);		// allocate 24B
		ptrs[3] = best_fit_alloc(32);		// allocate 32B
		// ptrs[4] = worst_fit_alloc(0);		// should be NULL
		ptrs[5] = best_fit_alloc(16);		// allocate 16B
		ptrs[6] = best_fit_alloc(61);		// allocate 64B
		// ptrs[7] = best_fit_alloc(1000);   // allocate too much space
		
		// Should join the middle-block with blocks before and after
		best_fit_dealloc(ptrs[3]);
		best_fit_dealloc(ptrs[1]);
		best_fit_dealloc(ptrs[2]);

		num = best_fit_count_extfrag(64);
	} else if ( algo == 1 ) {
		// Worst fit test cases

		worst_fit_memory_init(1024);	// initizae 1KB, worst fit

		ptrs[0] = worst_fit_alloc(8);		// allocate 8B
		ptrs[1] = worst_fit_alloc(16);		// allocate 16B
		ptrs[2] = worst_fit_alloc(14);		// allocate 24B
		ptrs[3] = worst_fit_alloc(32);		// allocate 32B
		// ptrs[4] = worst_fit_alloc(0);		// should be NULL
		ptrs[5] = worst_fit_alloc(16);		// allocate 16B
		ptrs[6] = worst_fit_alloc(61);		// allocate 64B
		// ptrs[7] = best_fit_alloc(1000);   // allocate too much space

		
		// Should join the middle-block with blocks before and after
		worst_fit_dealloc(ptrs[3]);
		worst_fit_dealloc(ptrs[1]);
		worst_fit_dealloc(ptrs[2]);

		num = worst_fit_count_extfrag(64);
	} else if ( algo == 2 ) {
		// Comparison Test Cases



	} else {
		fprintf(stderr, "Should not reach here!\n");
		exit(1);
	}

	printf("Expecting num = 2, received num = %d\n", num);

	return 0;
}
