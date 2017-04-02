/*SO 2012 TP2
Conway's Game of Life implementation 
Filipe Batista, 2006124931
M. Inês Coelho, 2004104311
Time Spent: 55h
*/

#include "header.h"

/*count the number of neighbour cells alive*/
int count_alive (int i, int j)
{
   int x, y, cont=0;

   for(x=i-1; x <= (i+1) ; x++)
   {
    	for(y=j-1; y <= (j+1) ; y++)
      	{
        	if ( (x==i) && (y==j) ) continue;
         	if ( (y<dimension) && (x<dimension) && (x>=0)   && (y>=0) )
         	{
             	cont += CELL(x,y);
         	}
      	}
	}
	return cont;
}

/*computes the next generation of a grid line*/
void evolve (int j)
{
	#ifdef DEBUG
		fprintf(stdout, "Evolve grid line: %d\n", j);
	#endif

	int i;
	for(i=0; i < dimension; i++)
	{
		int alive = count_alive(i, j); //number of neighbour cells alive
		if ( CELL(i,j) == 1) //cell is alive
		{
			if (alive > 3) TCELL(i, j) = 0; //cells dies from overcrowding
			else if (alive < 2) TCELL(i, j) = 0;  //cells dies from loneliness
			else TCELL(i, j) = 1; //cells keeps living	
		}
		else
		{
			if (alive == 3) TCELL(i, j) = 1; //cell is reborned
			else TCELL(i, j) = 0; //cell stays dead
		}
	}
}

/*copy a line from the temp grid into the game grid*/
int copy_grid ()
{
	int j;
	j = *write_grid;

	#ifdef DEBUG
		fprintf(stdout, "Copy grid line : %d\n", j);
	#endif

	int i;
	for (i=0; i< dimension; i++) {
		CELL(i,j) = TCELL(i,j); //copy a line from the grid
	}

	*write_grid +=1; //increase writing position
	if(*write_grid == dimension) *write_grid = 0; //return writing position to 0 at the end of the grid

	return j;
}