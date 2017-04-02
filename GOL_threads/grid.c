/*SO 2012 TP3
Conway's Game of Life implementation with threads
Filipe Batista, 2006124931
M. Inês Coelho, 2004104311
Time Spent: 50h
*/

#include "header.h"

/*initiate arrays and initiate variables*/
int init() 
{
	grid_a = malloc (sizeof(int*)*SIZE);
	if (grid_a == NULL) {
		fprintf(stderr, "Insuficient memory to create grid A!");
		return 0;
	}
	
	grid_b = malloc (sizeof(int*)*SIZE);
	if (grid_b == NULL) {
		fprintf(stderr, "Insuficient memory to create grid B!");
		return 0;
	}

	threads_set = malloc (THREADS_NUMBER * sizeof(int));
	if (threads_set == NULL) {
		fprintf(stderr, "Insuficient memory to create threads set!");
		return 0;
	}

	threads_id = malloc (THREADS_NUMBER * sizeof(int));
	if (threads_id == NULL) {
		fprintf(stderr, "Insuficient memory to create threads id!");
		return 0;
	}

	mpf_qtt = malloc ((duration/snapsh_freq) * sizeof(int));
	if (mpf_qtt == NULL) {
		fprintf(stderr, "Insuficient memory to create mpf array!");
		return 0;
	}

	mpf_copy = malloc ((duration/snapsh_freq) * sizeof(int));
	if (mpf_copy == NULL) {
		fprintf(stderr, "Insuficient memory to create mpf array!");
		return 0;
	}

	workers_info = malloc (THREADS_NUMBER * sizeof(worker_st));
	if (workers_info == NULL) {
		fprintf(stderr, "Insuficient memory to create workers info array!");
		return 0;
	}

	int i;
	for(i=0; i<THREADS_NUMBER; i++){
		pthread_mutex_init(&workers_info[i].mutex,NULL);
		pthread_cond_init(&workers_info[i].cond,NULL);
	 	workers_info[i].generation = 0;
	}

	//initialize arrays
	memset(grid_a, -1, sizeof(int*)*SIZE); //game grid A at -1
	memset(grid_b, 0, sizeof(int*)*SIZE); //game grid B at 0
	memset(mpf_qtt, 0, sizeof(int*)*(duration/snapsh_freq)); 
	memset(mpf_copy, 0, sizeof(int*)*(duration/snapsh_freq)); 

	next_snapshot = snapsh_freq;

	#ifdef DEBUG
		fprintf(stdout, "Arrays and semaphores sucessfully created!\n");
	#endif

	return 0;
}

/*print the entire grid with an X in the cells and . at the background*/
void print_grid (int gen, int * grid) 
{
	//printf("%ld\n", &grid[0]);
	int i,j;
	fprintf (stdout, "\nGENERATION %d\n", gen);
	for (i=0; i< dimension; i++) {
		for (j=0; j< dimension; j++)
			if (grid[OFFSET]  == 1)
				fprintf (stdout, "X");
			else if (grid[OFFSET]  == -1)
				fprintf (stdout, "-");
			else
				fprintf (stdout, ".");
		fprintf(stdout, "\n");
	}
}

/*release all the used resources*/
void terminate() 
{
	free(grid_a);
	free(grid_b);
	free(workers_info);
	free(threads_set);
	free(threads_id);
	free(mpf_qtt);
	free(mpf_copy);
}
