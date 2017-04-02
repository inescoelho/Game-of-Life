/*SO 2012 TP3
Conway's Game of Life implementation with threads
Filipe Batista, 2006124931
M. Inês Coelho, 2004104311
Time Spent: 50h
*/

#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
#include <pthread.h>


//#define DEBUG // comment this line to remove debug msgs
#define SNAPSHOTS // comment this line to remove snapshots
//#define TIME //comment this line to remove time count
#define N 3
#define PATTERN_NUMBER 5 
#define THREADS_NUMBER dimension/N //threads number
#define LEFTOVERS dimension%N //leftover lines
#define SIZE (dimension)*(dimension)
#define CELL(I,J) grid_a[dimension*(I)+(J)]
#define TAM 50 //tamanho max do nome do ficheiro
#define OFFSET (i*dimension + j)

#ifndef	MAP_FILE	/* 44BSD defines this & requires it to mmap files */
#define	MAP_FILE	0	/* to compile under systems other than 44BSD */
#endif

typedef enum { false, true } bool;

/*Structure used to store mutex, condition variables and info of each thread*/
typedef struct worker_struct{
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	int generation; 
}worker_st;

int * grid_a, * grid_b, *threads_id, *mpf_qtt, *mpf_copy , next_snapshot;
int duration, snapsh_freq, dimension;
int fd; //file descriptor
char nome[TAM], file_name[TAM], mutex_name[15], *mpf;
pthread_t *threads_set;
struct worker_struct *workers_info;

//grid.c
int init();
void print_grid (int gen, int * grid);
void terminate() ;
//worker.c
void *worker(void *id);
int count_alive (int * grid, int i, int j);
void evolve (int * grid_princ, int * grid_temp, int k, int qtt);
//patterns.c
int create_world ();
int create_patterns(int * pattern_qtt) ;
void draw_block (int x_pos, int y_pos);
void draw_glider (int x_pos, int y_pos, int type;);
void draw_spaceship (int x_pos, int y_pos, int type;);
void draw_pulsar (int x_pos, int y_pos, int type);
void fill_blanks(int x_pos, int y_pos, int type) ;
//files.c
void create_name();
void init_mmap(int gen);
void copy_mmpf(int * grid, int m, int p);
void close_mmpf();
void memory_mapped_files(int id, int * grid, int gen, int m, int p);

#endif