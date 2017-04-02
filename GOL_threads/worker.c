/*SO 2012 TP3
Conway's Game of Life implementation with threads
Filipe Batista, 2006124931
M. Inês Coelho, 2004104311
Time Spent: 50h
*/

#include "header.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

/*calculate 3-4 for lines, according to the thread id, for the duration of the game*/
void * worker(void *i) {
	int gen, line1, line2, line3, qtt, id_before, id_after;
	int *id = i;
	int *grid_array[2] = {grid_a, grid_b};

	//initiate first memory mapped file
	#ifdef SNAPSHOTS
		init_mmap(snapsh_freq);
	#endif

	//if it is the last thread and there is more than 1 line leftover
	if ((*id == THREADS_NUMBER-1) && (LEFTOVERS !=0)) 
	{
		line1 = 0 + *id*N;
		line2 = 1 + *id*N;
		line3 = 3 + *id*N;
		qtt = 2; 
		id_before = *id -1;
		id_after = 0;
	}

	else if ((*id == 0) && (LEFTOVERS == 2)) 
	{
		line1 = dimension-1;
		line2 = 0 + *id*N;
		line3 = 2 + *id*N;
		qtt = 2;
		id_before = THREADS_NUMBER-1;
		id_after = *id +1;
	} 

	else
	{
		line1 = 0 + *id*N;
		line2 = 1 + *id*N;
		line3 = 2 + *id*N;
		qtt = 1; 
		id_before = *id - 1;
		id_after = *id + 1;
	}

	for (gen = 0; gen < duration; gen++)
	{	
		//calculate next generation from the middle lines
		evolve(grid_array[(gen)%2], grid_array[(gen+1)%2], line2, qtt);

		//condition 1 - calculate left boundarie
		pthread_mutex_lock(&workers_info[id_before].mutex);

		while(workers_info[id_before].generation != workers_info[*id].generation && workers_info[id_before].generation != workers_info[*id].generation+1)
		{	
			pthread_cond_wait(&workers_info[id_before].cond, &workers_info[id_before].mutex);
		}

		evolve(grid_array[(gen)%2], grid_array[(gen+1)%2], line1, 1);
		pthread_mutex_unlock(&workers_info[id_before].mutex);

		//condition 2 - calculate right bondarie
		pthread_mutex_lock(&workers_info[id_after].mutex);

		while(workers_info[id_after].generation != workers_info[*id].generation && workers_info[id_after].generation != workers_info[*id].generation+1)
		{	
			pthread_cond_wait(&workers_info[id_after].cond, &workers_info[id_after].mutex);
		}

		evolve(grid_array[(gen)%2], grid_array[(gen+1)%2], line3, 1);
		pthread_mutex_unlock(&workers_info[id_after].mutex);

		//increment next generation and broadcast the condition
		pthread_mutex_lock(&workers_info[*id].mutex);
		workers_info[*id].generation ++;
		pthread_cond_broadcast(&workers_info[*id].cond);
		pthread_mutex_unlock(&workers_info[*id].mutex);

		//do a snapshot if it is the right generation
		#ifdef SNAPSHOTS
			if (workers_info[*id].generation == next_snapshot) 
				memory_mapped_files(*id, grid_array[(gen+1)%2], gen+1, line1, line3);
		#endif
	}

	pthread_exit(NULL);
	return NULL;
}

/*count the number of neighbour cells alive*/
int count_alive (int * grid, int i, int j)
{
   int x, y, a, b, cont=0;

   for(a=i-1; a <= (i+1) ; a++)
   {
   		if (a<0) x = dimension-1;
   		else x = a%dimension;

    	for(b=j-1; b <= (j+1) ; b++)
      	{
      		if (b<0) y = dimension-1;
   			else y = b%dimension;

        	if ( (x==i) && (y==j) ) continue;
            
            cont += grid[x*dimension + y];
      	}
	}
	return cont;
}

/*computes the next generation of a grid line*/
void evolve (int * grid_princ, int * grid_temp, int k, int qtt)
{
	int i, j;

	for (j=k; j<k+qtt; j++)	
	{
		#ifdef DEBUG
			fprintf(stdout, "Evolve grid line: %d\n", k);
		#endif

		for(i=0; i < dimension; i++)
		{
			int alive = count_alive(grid_princ, i, j); //number of neighbour cells alive

			if ( grid_princ[OFFSET] == 1) //cell is alive
			{
				if (alive > 3) grid_temp[OFFSET]  = 0; //cells dies from overcrowding
				else if (alive < 2) grid_temp[OFFSET]   = 0;  //cells dies from loneliness
				else grid_temp[OFFSET]   = 1; //cells keeps living	
			}
			else
			{
				if (alive == 3) grid_temp[OFFSET]  = 1; //cell is reborned
				else grid_temp[OFFSET]   = 0; //cell stays dead
			}
		}
	}
}
