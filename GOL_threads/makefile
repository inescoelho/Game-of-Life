#SO 2012 TP3
#Conway's Game of Life implementation with threads
#Filipe Batista, 2006124931
#M. Inês Coelho, 2004104311
#Time Spent: 50h

FLAGS  = -Wall -g -pthread -D_REENTRANT
CC     = gcc
PROG   = trab03
OBJS   = main.o grid.o patterns.o files.o worker.o

all:	${PROG}

clean:
	rm ${OBJS} ${PROG} *~
  
${PROG}:	${OBJS}
	${CC} ${FLAGS} ${OBJS} -o $@ 

.c.o:
	${CC} ${FLAGS} $< -c 

##########################

main.o:  header.h main.c
grid.o: header.h grid.c
patterns.o: header.h patterns.c
files.o: files.c header.h 
worker.o: worker.c header.h
trab02: main.o grid.o game.o patterns.o files.o worker.o

