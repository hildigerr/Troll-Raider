
CC = gcc
CFLAGS = -g -Wall
LFLAGS = -lm -lncurses
SRCS =  rmhv_stdlib.c cwin.c map.c player.c dat.c item.c game.c trollraider.c
DATA =  item.dat human.dat

OBJS = ${SRCS:.c=.o}

#Primary Targets
test: ${OBJS}
	${CC} ${CFLAGS} ${OBJS} ${LFLAGS}

trollraider: ${OBJS}
	${CC} -o ${@} ${OBJS} ${LFLAGS}

clean:
	rm -f a.out ${OBJS}

