/*SO 2012 TP2
Conway's Game of Life implementation 
Filipe Batista, 2006124931
M. Inês Coelho, 2004104311
Time Spent: 55h
*/

#include "header.h"

/*create file name to be used on snapshot files*/
void create_name() {
	time_t rawtime;
	struct tm * timeinfo;

	time ( &rawtime );
	timeinfo = localtime ( &rawtime );

	//file name is composed by YearMonthDay_HourMinute_GenerationNumber
	strftime(nome, sizeof(nome), "./snapshots/%Y%m%d_%H%M_Generation", timeinfo);
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

}

/*copy a line from the game grid into the memory mapped file*/
void copy_mmpf(int j) 
{
	int i;

	for (i=0; i< dimension; i++) {
		mpf[OFFSET] = CELL(i,j);
	}

}

/*terminate memory map and close snapshot file*/
void close_mmpf() {
	munmap(mpf, SIZE); //release memory mapped files
	close(fd); //close files
}