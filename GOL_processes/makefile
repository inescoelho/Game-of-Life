FLAGS  = -Wall -g
CC     = gcc
PROG   = trab02
OBJS   = main.o grid.o patterns.o game.o worker.o semaphore.o files.o

all:	${PROG}

clean:
	rm ${OBJS} ${PROG} *~
  
${PROG}:	${OBJS}
	${CC} ${FLAGS} ${OBJS} -o $@ -pthread

.c.o:
	${CC} ${FLAGS} $< -c 

##########################

main.o:  header.h main.c
grid.o: header.h grid.c
patterns.o: header.h patterns.c
game.o: header.h game.c
worker.o: worker.c header.h
semaphore.o: semaphore.c header.h
files.o: files.c header.h 
trab02: main.o grid.o game.o patterns.o worker.o semaphore.o files.o


