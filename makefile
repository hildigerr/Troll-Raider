
CC = gcc
CFLAGS = -g -Wall
LFLAGS = -lm -lncurses
SRCS =  src/rmhv_stdlib.c src/cwin.c src/map.c src/player.c src/dat.c src/item.c src/trollraider.c
DATA =  dat/item.dat dat/human.dat

OBJS = ${SRCS:.c=.o}

#Primary Targets
test: ${OBJS}
	${CC} ${CFLAGS} ${OBJS} ${LFLAGS}

trollraider: ${OBJS}
	${CC} -o ${@} ${OBJS} ${LFLAGS}

clean:
	rm -f a.out ${OBJS}

