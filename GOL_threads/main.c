/*SO 2012 TP3
Conway's Game of Life implementation with threads
Filipe Batista, 2006124931
M. Inês Coelho, 2004104311
Time Spent: 50h
*/

#include "header.h"

int main (void) 
{
	struct timeval start_time;
	struct timeval stop_time;
	float time_elapdsed;

	//data input by the user
	while(true) {
		fprintf(stdout, "Duration of the game (generations): ");
		scanf("%d", &duration);
		if ((duration <= 0) )fprintf(stdout, "Please, introduze a valid duration (>0).\n");
		else break;
	}

	while(true) {
		fprintf(stdout, "Frequency of snapshots (generations): ");
		scanf("%d", &snapsh_freq);
		if ((snapsh_freq < 1) || (snapsh_freq > duration) )
			fprintf(stdout, "Please, introduze a valid value for snapshot frequency (between 1 and %d).\n", duration);
		else break;
	}
	
	while(true) {
		fprintf(stdout, "Dimension of the grid (20-80): ");
		scanf("%d", &dimension);
		if ((dimension < 20) || (dimension > 80) )fprintf(stdout, "Please, introduze a number between 20 and 80!\n");
		else break;
	}

	//initiate game
	create_name(); //create name to be used in memory mapped files
	init();//initiate arrays and others variables
	create_world(); //create game world

	print_grid(0, grid_a); //print initial grid

	//copy initial grid into memory mapped files
	#ifdef SNAPSHOTS
		init_mmap(0);
		copy_mmpf(grid_a, 0, dimension);
		close_mmpf();
	#endif

	//initiate time counter
	#ifdef TIME
		gettimeofday( &start_time, NULL );
	#endif

	//create threads
	int i;
	for (i = 0; i < THREADS_NUMBER; i++) 
	{
		threads_id[i] = i;
		if (pthread_create(&threads_set[i], NULL, worker, (void*)&threads_id[i]) == -1)
		{
			perror("ERROR: pthread_create");
			exit(-1);
		}
	}

	//waits for the threads to die
	for (i = 0; i < THREADS_NUMBER; i++) {
		if (pthread_join(threads_set[i],NULL) == -1)
		{
			perror("ERROR: pthread_join");
			exit(-1);
		}
	}

	//stop time counter and calculate time elapsed
	#ifdef TIME
		gettimeofday( &stop_time, NULL );

		time_elapdsed = (float)(stop_time.tv_sec - start_time.tv_sec)*1000;
		time_elapdsed += (stop_time.tv_usec - start_time.tv_usec)/(float)1000;
		printf("Tempo: %.6fms \n ",time_elapdsed);
	#endif

	//choose the final grid
	int * grid;
	if (duration%2 == 1) grid = grid_b;
	else grid = grid_a;

	print_grid(duration, grid); //print final grid

	//copy grid final generation into a memory mapped file
	#ifdef SNAPSHOTS
		init_mmap(duration);
		copy_mmpf(grid, 0, dimension);
		close_mmpf();
	#endif

	//release memory resources
	terminate();

	return 0;
}
