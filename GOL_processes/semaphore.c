/*SO 2012 TP2
Conway's Game of Life implementation 
Filipe Batista, 2006124931
M. Inês Coelho, 2004104311
Time Spent: 55h
*/

#include "header.h"

/*initialize semaphores*/
void init() {

	//create semaphore stuct onto shared memory
	shmid3 = shmget(IPC_PRIVATE, sizeof(mem_struct), IPC_CREAT|0700);
	if (shmid3 < 1)
	exit(0);
	sh_mem = (mem_struct*) shmat(shmid3, NULL, 0);
	if (sh_mem < (mem_struct*) 1)
	exit(0);  

	//initiate semaphores
	sem_init(&sh_mem->sem_cont, 1, 0);
	cont = &sh_mem->sem_cont;
	sem_init(&sh_mem->sem_mutex, 1, 1);
	mutex = &sh_mem->sem_mutex;
	sem_init(&sh_mem->sem_mutex_grid, 1, 1);
	mutex_grid = &sh_mem->sem_mutex_grid;

	//initiate write and reading positions at 0
	sh_mem->read_gr = sh_mem->write_gr = 0;
	read_grid = &sh_mem->read_gr;
	write_grid = &sh_mem->write_gr;
}

/*terminate semaphores*/
void terminate_sem() {
	sem_close(cont);
	sem_close(mutex);
	sem_close(mutex_grid);
	shmctl(shmid3, IPC_RMID, NULL);
}

/*check if there is at least one half of the grid waiting to be re-written and, in that case, the producer waits 0,1s*/
void check_sem() {
	int check;
	struct timespec ts;
	struct timeval tv;

	sem_getvalue(cont, &check); //get semaphore cont value
	if (check <= dimension/2)  //if this value is less than one half of the grid, terminate
		return; 

	// get current time and add 0,1 second for timeout.
	gettimeofday(&tv, NULL);
	ts.tv_sec = tv.tv_sec;
	ts.tv_nsec = tv.tv_usec;
	ts.tv_nsec += 100000;   // 0,1sec
	if (ts.tv_nsec > 1000000)   
	{
    	ts.tv_sec++;
	    ts.tv_nsec -= 1000000;
	}
	ts.tv_nsec *= 1000;    /* convert to nanosecs */

	sem_timedwait(mutex_grid, &ts); //put semaphore mutex_grid on wait for 0,1s
}