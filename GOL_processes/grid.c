/*SO 2012 TP2
Conway's Game of Life implementation 
Filipe Batista, 2006124931
M. Inês Coelho, 2004104311
Time Spent: 55h
*/

#include "header.h"

/*create grid*/
int create_grid () 
{
	//create game grid on shared memory
	shmid1 = shmget(IPC_PRIVATE, dimension*dimension*sizeof(int), IPC_CREAT|0700);
	if (shmid1 < 1) exit(0);
	grid = (int*) shmat(shmid1, NULL, 0);

	#ifdef DEBUG
		fprintf(stdout, "Grid created! Total area: %d.\n", dimension*dimension);
	#endif


	//create temporary game grid on shared memory
	shmid2 = shmget(IPC_PRIVATE, dimension*dimension*sizeof(int), IPC_CREAT|0700);
	if (shmid2 < 1) exit(0);
	temp = (int*) shmat(shmid2, NULL, 0);

	#ifdef DEBUG
		fprintf(stdout, "Temp grid  created! Total area: %d.\n", dimension*dimension);
	#endif

	//initialize both grids: game grid at -1 and temp grid at 0
	int i,j;
	for (i=0; i< dimension; i++)
		for (j=0; j< dimension; j++) {
			CELL(i,j) = -1;
			TCELL(i,j) = 0;
		}

	return 0;
}

/*print the entire grid with an X in the cells and . at the background*/
void print_grid (int gen) 
{
	int i,j;
	fprintf (stdout, "\nGENERATION %d\n", gen);
	for (i=0; i< dimension; i++) {
		for (j=0; j< dimension; j++)
			if (CELL(i,j) == 1)
				fprintf (stdout, "X");
			else if (CELL(i,j) == -1)
				fprintf (stdout, "-");
			else
				fprintf (stdout, ".");
		fprintf(stdout, "\n");
	}
}

/*release all the used resources*/
void terminate() 
{
	shmctl(shmid1, IPC_RMID, NULL); //release game grid in shared memory
	shmctl(shmid2, IPC_RMID, NULL); //release temp grid in shared memory
	terminate_sem(); //terminate semaphores
}
