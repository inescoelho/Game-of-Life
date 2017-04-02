/*SO 2012 TP3
Conway's Game of Life implementation with threads
Filipe Batista, 2006124931
M. Inês Coelho, 2004104311
Time Spent: 50h
*/

#include "header.h"

pthread_mutex_t mutex_mpf = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cont_mpf = PTHREAD_COND_INITIALIZER;
pthread_cond_t cont_cont = PTHREAD_COND_INITIALIZER;

/*create file name to be used on snapshot files*/
void create_name() {
	time_t rawtime;
	struct tm * timeinfo;

	time ( &rawtime );
	timeinfo = localtime ( &rawtime );

	//file name is composed by YearMonthDay_HourMinute_GenerationNumber
	strftime(nome, sizeof(nome), "./snapshots/%Y%m%d_%H%M_Generation", timeinfo);
	//printf("%s\n", nome);
}

/*open snapshot file and initiate memory map*/
void init_mmap(int gen)
{
	//update generation number of snapshot file name
	sprintf(file_name, "%s%d.txt", nome, gen);
	#ifdef DEBUG
		fprintf(stdout, "Snapshot file created: %s\n", file_name);
	#endif

	//open output file and truncate
	if ( (fd = open(file_name, O_RDWR | O_CREAT | O_TRUNC, (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH))) < 0)
	{
		fprintf(stderr,"Unable to creat snapshot file for writing.\nPlease, verify the existence of a snapshots directory.\n");
		exit(1);
	}

	//set size of output file
	if (lseek(fd, SIZE - 1, SEEK_SET) == -1)
	{
		fprintf(stderr,"lseek error\n");
		exit(1);
	}
	if (write(fd, "", 1) != 1) //writing last byte of file
	{
		fprintf(stderr,"write error\n");
		exit(1);
	}

	//map the file into memory
	if ( (mpf = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == (caddr_t) -1)
	{
		fprintf(stderr,"mmap error for input\n");
		exit(1);
	}

	return;
}

/*copy a line from the game grid into the memory mapped file*/
void copy_mmpf(int * grid, int m, int p) 
{
	int i, j;

	for (i=m; i<= p; i++) 
		for (j=0; j< dimension; j++)
		{
			mpf[OFFSET] = grid[OFFSET];
		}

}

/*terminate memory map and close snapshot file*/
void close_mmpf() {
	munmap(mpf, SIZE); //release memory mapped files
	close(fd); //close files
}

/*wait for all the threads to reach the same generation and copy their info into the memory mapped file*/
void memory_mapped_files(int id, int * grid, int gen, int m, int p) 
{
	int indice;

	indice = gen/snapsh_freq-1;
	mpf_qtt[indice] ++;

	pthread_mutex_lock(&mutex_mpf);

	while(mpf_qtt[indice] != THREADS_NUMBER)
		pthread_cond_wait(&cont_mpf, &mutex_mpf);
			
	if (m == dimension-1)
	{
		copy_mmpf(grid, dimension-1,dimension-1);
		copy_mmpf(grid, 0, p);
	}

	else
		copy_mmpf(grid, m, p);

	mpf_copy[indice] ++;

	if (mpf_copy[indice] == THREADS_NUMBER) {
		close_mmpf();
		next_snapshot += snapsh_freq;
		if (next_snapshot < duration)
			init_mmap (next_snapshot);
		else
			next_snapshot = 0;
		pthread_cond_broadcast(&cont_cont);
	}

	pthread_cond_broadcast(&cont_mpf);

	while(mpf_copy[indice] != THREADS_NUMBER)
		pthread_cond_wait(&cont_cont, &mutex_mpf);

	pthread_mutex_unlock(&mutex_mpf);

	return;
}