/*SO 2012 TP2
Conway's Game of Life implementation 
Filipe Batista, 2006124931
M. Inês Coelho, 2004104311
Time Spent: 55h
*/

#include "header.h"

/*Divides the grid into the several processes, during the corresponding number of generations, and compute the next available line*/
void producer(int index) {
	int i, gen;

	for (gen = 1; gen <= duration; gen++)
	{
	    for (i = index; i < dimension; i+=PROCESSES_NUMBER) {
	    	sem_wait(mutex_grid); //only one process can read the number of the next line at a time
	    
	    	if(*read_grid == dimension) *read_grid = 0; //return reading position to 0 at the end of the grid
	    	evolve(*read_grid); //compute the next line
	    	
			//to assure that the consumer doesn't re-write all the grid before the next generation is calculated
			if (*read_grid != 0)	sem_post(cont); //semaohore cont is only updated after the first line
			if (*read_grid == dimension-1)	
			{
				sem_post(cont); //in the end of the grid, the first line is accounted
				*read_grid = 0; //return reading position to 0 at the end of the grid
			}
			else
				*read_grid +=1; //increases next reading line value

	    	check_sem(); //verifies if there is more than 1/2 of the grid waiting to be re-written and, in that case, waits 0,1s
	    	sem_post(mutex_grid);
    	}
 	}
}

/*Waits for lines which the next generation has been computed and copy them into the game grid
When is "snapshot generation" copy the new lines into a memory mapped file*/
void consumer() {
	int i, line, gen, ss_gen_value;
	bool ss_gen = false; //flag that is activated when is supposed to take a snapshot from this generation

	ss_gen_value = snapsh_freq; //next generation to take a snapshot from
	init_mmap(ss_gen_value); //initiate a memory mapped file for the first snapshot generation

	for (gen = 1; gen <= duration; gen++)
	{
		if (gen == ss_gen_value || gen == duration) { //confirm if is a "snapshot generation"
			ss_gen_value += snapsh_freq; //increases the value of the next "snapshot generation"
			if(ss_gen_value > duration) ss_gen_value = duration;
			ss_gen = true; //activates flag 
		}

	    for (i = 0; i < dimension; i++) {
	    	sem_wait(cont);
			Ssem_wait(mutex);
	    	line = copy_grid(); //copy next line from temp grid into game grid
	    	
	    	sem_post(mutex);
	    	
	    	if (ss_gen) { //if is snapshot geneation, copy game grid line into memory mapped file
	    		copy_mmpf(line);
	    	}
	    }

	    if (ss_gen) {
	   		ss_gen = false; //inactivates flag 
	   		close_mmpf(); //close this memory mapped file
	   		if (gen < duration)
	   			init_mmap(ss_gen_value); //initiate a memory mapped file for the next snapshot generation
	    }
	}
}
