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
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

/* non-system provided header files. 
   Do not include more user-defined header files here
 */
#include "mem.h"
enum test_type {best_fit, worst_fit};

void clear_mem(void** pointers, int length, enum test_type best_or_worst){
	for(int i = 0; i < length; ++i){
        if(pointers[i] == NULL){
            
        }
		else if(best_or_worst == best_fit){
			best_fit_dealloc(pointers[i]);
		}
		else{
			worst_fit_dealloc(pointers[i]);
		}
	}
}

//allocate then deallocate same memory ensure pointer doesn't change
void test_1(void** pointers, enum test_type best_or_worst){
	void* allocated;
	pointers[0] = NULL;
	for(int i = 0; i < 10; ++i){
		switch(best_or_worst){
			case best_fit: 
				allocated = best_fit_alloc(4);
				if(i != 0 && allocated != pointers[0]){
					printf("ERROR\n");
				}
				pointers[0] = allocated;
				best_fit_dealloc(allocated);
				break;
			case worst_fit:
				allocated = worst_fit_alloc(4);
				if(i != 0 && allocated != pointers[0]){
					printf("ERROR\n");
				}
				pointers[0] = allocated;
				worst_fit_dealloc(allocated);
				break;
		}
	}
}

//allocate 10 space deallocate all and ensure no memory fragmentation
void test_2(void** pointers, enum test_type best_or_worst){
	int fragmentation;
	for(int i = 0; i < 10; ++i){
		switch(best_or_worst){
			case best_fit:
				pointers[i] = best_fit_alloc(4);
				break;
			case worst_fit:
				pointers[i] = worst_fit_alloc(4);
				break;
		}
	}
	clear_mem(pointers, 10, best_or_worst);
	switch(best_or_worst){
		case best_fit:
			fragmentation = best_fit_count_extfrag(4);
			break;
		case worst_fit:
			fragmentation = worst_fit_count_extfrag(4);
			break;
	}
	if(fragmentation != 0){
		printf("ERROR FRAGMENTION IN CLEARED MEMORY\n");
	}
}


//assign 10 random sized blocks(less then size 100) then remove 5 blocks(1, 3, 4, 6, 8 )
//then add 3 more of size 24
//check frag for sizes 1 25 50 then clear mem
//repeat 100 times to average results 
void test_3(void** pointers, enum test_type best_or_worst){
	int random_size;
	double frag_size1 = 0, frag_size25 = 0, frag_size50 = 0;
	for(int i  = 0; i < 100; ++i){
		random_size = rand() % 100 + 1;
		for(int j = 0; j < 12; ++j){
			switch(best_or_worst){
				case best_fit:
					pointers[j] = best_fit_alloc(random_size);
					break;
				case worst_fit:
					pointers[j] = worst_fit_alloc(random_size);
					break;
			}
		}
		switch(best_or_worst){
			case best_fit:
				best_fit_dealloc(pointers[0]);
				best_fit_dealloc(pointers[2]);
				best_fit_dealloc(pointers[3]);
				best_fit_dealloc(pointers[5]);
				best_fit_dealloc(pointers[7]);
                pointers[0] = best_fit_alloc(24);
                pointers[2] = best_fit_alloc(24);
                pointers[3] = best_fit_alloc(24);
                pointers[5] = best_fit_alloc(24);
                pointers[7] = best_fit_alloc(24);
				frag_size1 += best_fit_count_extfrag(4);
				frag_size25 += best_fit_count_extfrag(25);
				frag_size50 += best_fit_count_extfrag(50);
				break;
			case worst_fit:
				worst_fit_dealloc(pointers[0]);
				worst_fit_dealloc(pointers[2]);
				worst_fit_dealloc(pointers[3]);
				worst_fit_dealloc(pointers[5]);
				worst_fit_dealloc(pointers[7]);
                
                pointers[0] = worst_fit_alloc(24);
                pointers[2] = worst_fit_alloc(24);
                pointers[3] = worst_fit_alloc(24);
                pointers[5] = worst_fit_alloc(24);
                pointers[7] = worst_fit_alloc(24);
				frag_size1 += worst_fit_count_extfrag(4);
				frag_size25 += worst_fit_count_extfrag(25);
				frag_size50 += worst_fit_count_extfrag(50);
				break;
		}
        clear_mem(pointers, 12, best_or_worst);
	}
	printf("Test Case 3: fragmentation size 1 %lf fragmantation size 25 %lf fragmantation size 50 %lf\n",frag_size1, frag_size25, frag_size50);
}

//fill memory with blocks of size 100 
//deallocate 1 3 4 6
// allocate 3 random sized between 1 and 100 
//check frag 1 25 50
//run 100 times
void test_4(void** pointers, enum test_type best_or_worst){
	int number;
	int random_number;
	void* most_recent_alloc;
	double frag_size1 = 0, frag_size25 = 0 , frag_size50 = 0;
	for(int i = 0; i < 100 ; ++i){
		number = 1;
		if(best_or_worst == best_fit){
			pointers[0] = best_fit_alloc(100);
			most_recent_alloc = pointers[0];
		}
		else{
			pointers[0] = worst_fit_alloc(100);
			most_recent_alloc = pointers[0];
		}
		while(most_recent_alloc != NULL){
			if(best_or_worst == best_fit){
				pointers[number] = best_fit_alloc(100);
			}
			else{
				pointers[number] = worst_fit_alloc(100);
				
			}
            most_recent_alloc = pointers[number];
            number++;
		}
		if(best_or_worst == best_fit){
			best_fit_dealloc(pointers[0]);
			best_fit_dealloc(pointers[2]);
            best_fit_dealloc(pointers[3]);
            best_fit_dealloc(pointers[5]);
            random_number = rand() % 100 + 1;
			pointers[0] = best_fit_alloc(random_number);
			random_number = rand() % 100 + 1;
			pointers[2] = best_fit_alloc(random_number);
			random_number = rand() % 100 + 1;
			pointers[3] = best_fit_alloc(random_number);	
			random_number = rand() % 100 + 1;
			pointers[5] = best_fit_alloc(random_number);
			frag_size1 += best_fit_count_extfrag(4);
			frag_size25 += best_fit_count_extfrag(25);
			frag_size50 += best_fit_count_extfrag(50);	
		}
		else{
			worst_fit_dealloc(pointers[0]);
			worst_fit_dealloc(pointers[2]);
			worst_fit_dealloc(pointers[3]);
			worst_fit_dealloc(pointers[5]);
			random_number = rand() % 100 + 1;
			pointers[0] = worst_fit_alloc(random_number);
			random_number = rand() % 100 + 1;
			pointers[2] = worst_fit_alloc(random_number);
			random_number = rand() % 100 + 1;
			pointers[3] = worst_fit_alloc(random_number);
			random_number = rand() % 100 + 1;
			pointers[5] = worst_fit_alloc(random_number);
			frag_size1 += worst_fit_count_extfrag(4);
			frag_size25 += worst_fit_count_extfrag(25);
			frag_size50 += worst_fit_count_extfrag(50);
		}
		clear_mem(pointers, number - 1, best_or_worst);
	}

	printf("Test Case 4: fragmentation size 1 %lf fragmantation size 25 %lf fragmantation size 50 %lf\n",frag_size1, frag_size25, frag_size50);
}

// fill with size 4
// deallocte 10 places between 1 and 100
// allocate 10 blocks of size 2
//check frag 1 25 50
//repeat 100 times
void test_5(void** pointers, enum test_type best_or_worst){
	int number;
	int random_number;
	void* most_recent_alloc;
	double frag_size1 = 0, frag_size25 = 0, frag_size50 = 0;
	for(int i = 0; i < 100 ; ++i){
		number = 1;
		if(best_or_worst == best_fit){
			pointers[0] = best_fit_alloc(4);
			most_recent_alloc = pointers[0];
		}
		else{
			pointers[0] = worst_fit_alloc(4);
			most_recent_alloc = pointers[0];
		}
		while(most_recent_alloc != NULL){
			if(best_or_worst == best_fit){
				pointers[number] = best_fit_alloc(4);
			}
			else{
				pointers[number] = worst_fit_alloc(4);
			}
			most_recent_alloc = pointers[number];
			number++;
		}
		if(best_or_worst == best_fit){
			best_fit_dealloc(pointers[0]);
			best_fit_dealloc(pointers[2]);
			best_fit_dealloc(pointers[3]);
			best_fit_dealloc(pointers[5]);
			best_fit_dealloc(pointers[6]);
			best_fit_dealloc(pointers[7]);

			random_number = rand() % 4 + 1;
			pointers[0] = best_fit_alloc(random_number);
			random_number = rand() % 4 + 1;
			pointers[2] = best_fit_alloc(random_number);
			random_number = rand() % 4 + 1;
			pointers[3] = best_fit_alloc(random_number);
            random_number = rand() % 4 + 1;
            pointers[5] = best_fit_alloc(random_number);
            random_number = rand() % 4 + 1;
            pointers[6] = best_fit_alloc(random_number);
            random_number = rand() % 4 + 1;
            pointers[7] = best_fit_alloc(random_number);
			frag_size1 += best_fit_count_extfrag(4);
			frag_size25 += best_fit_count_extfrag(25);
			frag_size50 += best_fit_count_extfrag(50);	
		}
		else{
			worst_fit_dealloc(pointers[0]);
			worst_fit_dealloc(pointers[2]);
			worst_fit_dealloc(pointers[3]);
			worst_fit_dealloc(pointers[5]);
			worst_fit_dealloc(pointers[6]);
			worst_fit_dealloc(pointers[7]);
			random_number = rand() % 4 + 1;
			pointers[0] = worst_fit_alloc(random_number);
			random_number = rand() % 4 + 1;
			pointers[2] = worst_fit_alloc(random_number);
			random_number = rand() % 4 + 1;
			pointers[3] = worst_fit_alloc(random_number);
            random_number = rand() % 4 + 1;
            pointers[5] = worst_fit_alloc(random_number);
            random_number = rand() % 4 + 1;
            pointers[6] = worst_fit_alloc(random_number);
            random_number = rand() % 4 + 1;
            pointers[7] = worst_fit_alloc(random_number);
			frag_size1 += worst_fit_count_extfrag(4);
			frag_size25 += worst_fit_count_extfrag(25);
			frag_size50 += worst_fit_count_extfrag(50);
		}
		clear_mem(pointers, number - 1, best_or_worst);
	}
	printf("Test Case 5: fragmentation size 1 %lf fragmantation size 25 %lf fragmantation size 50 %lf\n",frag_size1, frag_size25, frag_size50);
}


//fill with size 25 
//deallocate 3 place between 1 and 40
//allocate 3 blocks of size 1 - 25
//check frag 1 25 50
//repeat 100 times 
void test_6(void** pointers, enum test_type best_or_worst){
	int number;
	int random_number;
	void* most_recent_alloc;
	double frag_size1, frag_size25, frag_size50 = 0;
	for(int i = 0; i < 100 ; ++i){
		number = 1;
		if(best_or_worst == best_fit){
			pointers[0] = best_fit_alloc(25);
			most_recent_alloc = pointers[0];
		}
		else{
			pointers[0] = worst_fit_alloc(25);
			most_recent_alloc = pointers[0];
		}
		while(most_recent_alloc != NULL){
			if(best_or_worst == best_fit){
				pointers[number] = best_fit_alloc(25);
			}
			else{
				pointers[number] = worst_fit_alloc(25);
			}
			most_recent_alloc = pointers[number];
			number++;
		}
		if(best_or_worst == best_fit){
			best_fit_dealloc(pointers[0]);
			best_fit_dealloc(pointers[2]);
			best_fit_dealloc(pointers[3]);
			random_number = rand() % 25 + 1;
			pointers[0] = best_fit_alloc(random_number);
			random_number = rand() % 25 + 1;
			pointers[2] = best_fit_alloc(random_number);
			random_number = rand() % 25 + 1;
			pointers[3] = best_fit_alloc(random_number);
			frag_size1 += best_fit_count_extfrag(4);
			frag_size25 += best_fit_count_extfrag(25);
			frag_size50 += best_fit_count_extfrag(50);	
		}
		else{
			worst_fit_dealloc(pointers[0]);
			worst_fit_dealloc(pointers[2]);
			worst_fit_dealloc(pointers[3]);
			random_number = rand() % 25 + 1;
			pointers[0] = worst_fit_alloc(random_number);
			random_number = rand() % 25 + 1;
			pointers[2] = worst_fit_alloc(random_number);
			random_number = rand() % 25 + 1;
			pointers[3] = worst_fit_alloc(random_number);
			frag_size1 += worst_fit_count_extfrag(4);
			frag_size25 += worst_fit_count_extfrag(25);
			frag_size50 += worst_fit_count_extfrag(50);
		}
		clear_mem(pointers, number - 1, best_or_worst);
	}
	printf("Test Case 6: fragmentation size 1 %lf fragmantation size 25 %lf fragmantation size 50 %lf\n",frag_size1, frag_size25, frag_size50);
}

//allocate 15 random sized blocks
//deallocate them
//repeat 1000 times 
//time how long it takes
void test_7(void** pointers, enum test_type best_or_worst){
	double g_time[2];
	int random_size = 0;
	struct timeval tv;
	gettimeofday(&tv, NULL);
	g_time[0] = (tv.tv_sec) + tv.tv_usec/1000000.;
	for(int i = 0; i < 1000; ++i){
		for(int j = 0; j < 15; ++j){
			random_size = rand() % 100 + 1;
			switch(best_or_worst){
				case best_fit:
					pointers[j] = best_fit_alloc(random_size);
					break;
				case worst_fit:
					pointers [j] = worst_fit_alloc(random_size);
					break;
			}
		}
		for(int j = 0 ; j < 15; ++j){
			switch(best_or_worst){
				case best_fit:
					best_fit_dealloc(pointers[j]);
					break;

				case worst_fit:
					worst_fit_dealloc(pointers[j]);
			}
		}
	}

	gettimeofday(&tv, NULL);
    g_time[1] = (tv.tv_sec) + tv.tv_usec/1000000.;
	printf("System execution time: %.6lf seconds\n", \
            g_time[1] - g_time[0]);
}

int main(int argc, char *argv[])
{
	 int algo = 0; // default algorithm to test is best fit  
	 void** pointers = malloc(1000 * sizeof(void*));
	 enum test_type best_or_worst;
    for(int i = 0; i < 1000; ++i){
        pointers[i] = NULL;
    }
    
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
		best_or_worst = best_fit;
		best_fit_memory_init(2048);	// initizae 2KB, best fit
	} else if ( algo == 1 ) {
		best_or_worst = worst_fit;
		worst_fit_memory_init(2048);	// initizae 2KB, worst fit
		//num = worst_fit_count_extfrag(4);	
	} else {
		fprintf(stderr, "Should not reach here!\n");
		exit(1);
	}

	test_1(pointers, best_or_worst);
	//print_mem(algo);
    test_2(pointers, best_or_worst);
	//print_mem(algo);
    test_3(pointers, best_or_worst);
    //print_mem(algo);
	test_4(pointers, best_or_worst);
    //print_mem(algo);
    test_5(pointers, best_or_worst);
    //print_mem(algo);
	test_6(pointers, best_or_worst);
	//print_mem(algo);
	test_7(pointers, best_or_worst);
    print_mem(algo);
	return 0;
}
