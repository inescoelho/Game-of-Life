/*SO 2012 TP2
Conway's Game of Life implementation 
Filipe Batista, 2006124931
M. Inês Coelho, 2004104311
Time Spent: 55h
*/

#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h> 
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/fcntl.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <semaphore.h>
#include <errno.h>
#include <time.h>

#define DEBUG // remove this line to remove debug msgs
#define PATTERN_NUMBER 5 
#define PROCESSES_NUMBER 4
#define CELL(I,J) (grid[dimension*(I)+(J)])
#define TCELL(I,J) (temp[dimension*(I)+(J)])
#define SIZE (dimension)*(dimension)
#define OFFSET (i*dimension + j)
#define TAM 50

#ifndef	MAP_FILE	/* 44BSD defines this & requires it to mmap files */
#define	MAP_FILE	0	/* to compile under systems other than 44BSD */
#endif

typedef enum { false, true } bool;

typedef struct {
	sem_t sem_cont; 
	sem_t sem_mutex;
	sem_t sem_mutex_grid;
	int read_gr, write_gr;
} mem_struct;

int shmid1, shmid2, shmid3;
int * grid, * temp, fd;
char nome[TAM], file_name[TAM], *mpf;
int duration, snapsh_freq, dimension;
mem_struct *sh_mem;
sem_t *cont, *mutex, *mutex_grid;
int *read_grid, *write_grid;
pid_t id[PROCESSES_NUMBER];

void create_name();
void init();
void terminate_sem() ;
void producer(int index);
void consumer(); 
int create_world ();
int create_patterns(int * pattern_qtt) ;
void draw_block (int x_pos, int y_pos);
void draw_glider (int x_pos, int y_pos, int type;);
void draw_spaceship (int x_pos, int y_pos, int type;);
void draw_pulsar (int x_pos, int y_pos, int type);
void fill_blanks(int x_pos, int y_pos, int type) ;
int create_grid ();
void print_grid (int gen);
int count_alive (int i, int j);
void evolve (int j);
int copy_grid ();
void terminate ();
void init_mmap(int gen);
void copy_mmpf (int line);
void check_sem();
void close_mmpf();

#endif