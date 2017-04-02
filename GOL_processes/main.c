/*SO 2012 TP2
Conway's Game of Life implementation 
Filipe Batista, 2006124931
M. Inês Coelho, 2004104311
Time Spent: 55h
*/

#include "header.h"


int main (void) 
{
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

	//initiate memory
	init(); //initiate semaphores
	create_name(); //create name to be used in memory mapped files
	create_grid(); //create grid with 0's
	create_world(); //populate world

	#ifdef DEBUG
		print_grid(0); //print initial grid
	#endif
	
	//copy grig generation 0 into a memory mapped file
	init_mmap(0);
	int line;
	for (line = 0; line < dimension; line++) 
		copy_mmpf(line);

	// create processes
	int i;
	for(i=0;i<PROCESSES_NUMBER;i++)
	{
		id[i] = fork();

		if (id[i]== 0) {
	    	producer(i); //start producer function
	    	exit(0);
	    }
	}

	consumer(); //

	//wait for children
	int j;
	for (j = 0; j < PROCESSES_NUMBER; j++) 
	{
		int status;
		while (-1 == waitpid(id[j], &status, 0));
		
		if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
			printf("Process %d (pid %d ) failed\n",j,id[j]);
			exit(1);
		}
	}

	print_grid(duration); //print final grid

	terminate(); //release all resources

	return 0;
}

